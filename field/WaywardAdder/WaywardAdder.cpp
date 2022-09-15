#include "daisy_field.h"
#include "daisysp.h"
#include <array>
#include <algorithm>

using namespace daisy;
using namespace daisysp;

#define NUM_BUFFERS 8
#define BUFFER_SIZE (48000 * 5)

DaisyField hw;

struct AdderParams
{
    int cvId;
    float knobVal;
    float cvInNorm;
    float cvOutNorm;
    int semitoneAdd;
};

AdderParams _knob0Params;
AdderParams _knob1Params;

int _rangeOctaves = 2;
float _semitoneRange = 12.0f;

const int kRangeOctavesMax = 3;
int foo;
const float kInputCvMin = -1.0f; // we are normalized to 0->1 on CV
const float kInputCvMax = 1.0f;
void ReadCV()
{
    _knob0Params.cvInNorm = hw.GetCvValue(_knob0Params.cvId);
    _knob1Params.cvInNorm = hw.GetCvValue(_knob1Params.cvId);
	foo = 3;
}

void Controls()
{
    hw.ProcessAllControls();
    _knob0Params.knobVal = hw.GetKnobValue(_knob0Params.cvId);
    _knob1Params.knobVal = hw.GetKnobValue(_knob1Params.cvId);
}

void doVoltageAdd(AdderParams &params)
{
	// convert knob value to semitones, spread over the octave range.
	static int semitones = 0.0f;

	// quantize our input to semitones by casting to int.
	semitones = (int) (params.knobVal * (_semitoneRange+1));
	params.semitoneAdd = abs(semitones); // keep track of this so we can display it.

	// turn that back into voltage - 1/12v per semitone
	// however, our input is normalized with 0->1 => 0->5V, so we need to map onto that range to add them.
	float voltsToAdd =	((float)semitones) / 12.0f / 5.0f;

	// now we're in the same normalized (0-1) space, so add voltages.
	float newVoltNorm = fclamp(params.cvInNorm + voltsToAdd,0,1);

	// cache that value so we can display it.
	params.cvOutNorm = newVoltNorm;

}

void AddKnobCV()
{
	doVoltageAdd(_knob0Params);
	doVoltageAdd( _knob1Params);

	// cv out is a 0->5V across the full int range of 0->4095, so each semitone is 1/12V * 4095/5.0
	hw.SetCvOut1((int)(_knob0Params.cvOutNorm * 4095.0f));
	hw.SetCvOut2((int)(_knob1Params.cvOutNorm * 4095.0f));
}



void WriteCV() {
    // todo
}

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    Controls();
    ReadCV();
    AddKnobCV();
    WriteCV();

    for(size_t i = 0; i < size; i++)
    {
        // Zero samples prior to summing
        out[0][i] = 0.f;
        out[1][i] = 0.f;
    }
}

void UpdateDisplay()
{
    static const char headerRow[17] = "  CH1      CH2  ";
	static char row[17] = "  0000     0000 ";

    hw.display.Fill(false);
    hw.display.SetCursor(0, 0);
    hw.display.WriteString(headerRow, Font_7x10, true);

	sprintf(row, "  %04d    %04d", (int)(_knob0Params.knobVal * 100.0f), (int)(_knob1Params.knobVal * 100.0f));
	hw.display.SetCursor(0, 10);
	hw.display.WriteString(row, Font_7x10, true);

	sprintf(row, "  %04d    %04d", (int)(_knob0Params.cvInNorm * 1000.0f), (int)(_knob1Params.cvInNorm * 1000));
	hw.display.SetCursor(0, 20);
	hw.display.WriteString(row, Font_7x10, true);

	sprintf(row, "   +%02d     +%02d", _knob0Params.semitoneAdd, _knob1Params.semitoneAdd);
	hw.display.SetCursor(0, 30);
	hw.display.WriteString(row, Font_7x10, true);

	sprintf(row, "  %04d    %04d", (int)(_knob0Params.cvOutNorm * 1000.0f), (int)(_knob1Params.cvOutNorm * 1000));
	hw.display.SetCursor(0, 40);
	hw.display.WriteString(row, Font_7x10, true);

	hw.display.Update();
}

void InitConstants()
{
	_semitoneRange = 12.0f * _rangeOctaves;
    _knob0Params.cvId = 0;
    _knob1Params.cvId = 1;
}

int main(void)
{
    hw.Init(); // Don't try to use SDRAM until after this Init function
    InitConstants();
    hw.StartAdc();
    hw.StartAudio(AudioCallback);
    while(true)
    {
        UpdateDisplay();
        System::Delay(10);
    }
}
