#include "daisy_field.h"
#include "daisysp.h"
#include "AdderStepParams.h"
#include <array>
#include <algorithm>

using namespace daisy;
using namespace daisysp;

DaisyField hw;

AdderStepParams UpperRowParams[8];
AdderStepParams *SelectedStep;
size_t UpperSelectDex = 0;

size_t upperKeyLeds[] = {
		DaisyField::LED_KEY_B1,
		DaisyField::LED_KEY_B2,
		DaisyField::LED_KEY_B3,
		DaisyField::LED_KEY_B4,
		DaisyField::LED_KEY_B5,
		DaisyField::LED_KEY_B6,
		DaisyField::LED_KEY_B7,
		DaisyField::LED_KEY_B8
};

size_t upperKeyButtons[] =
		{
			DaisyField::LED_KEY_A8,
			DaisyField::LED_KEY_A7,
			DaisyField::LED_KEY_A6,
			DaisyField::LED_KEY_A5,
			DaisyField::LED_KEY_A4,
			DaisyField::LED_KEY_A3,
			DaisyField::LED_KEY_A2,
			DaisyField::LED_KEY_A1
		};

void ReadCV()
{
	SelectedStep->ReadCVInput(hw);
}

void SetUpperSelection(size_t newDex)
{
	UpperSelectDex = newDex;
	SelectedStep = &(UpperRowParams[UpperSelectDex]);
	SelectedStep->NotifySelectedStep(hw);
}

void Controls()
{
    hw.ProcessAllControls();
	size_t selectedKeyDex = 8;
	for (size_t i = 0; i < 8; i++)
	{
		if (hw.KeyboardRisingEdge(upperKeyButtons[i]))
		{
			selectedKeyDex = i;
		}
	}
	if (selectedKeyDex < 8)
	{
		SetUpperSelection(selectedKeyDex);
	}
	SelectedStep->ProcessStepControls(hw);
}

void AddKnobCV()
{
	SelectedStep->CalcVoltageAdd();
}

void WriteCV()
{
	SelectedStep->SendCVOut(hw);
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
    static const char headerRow[20] = " CH1  CH2";
	static char row[20] = "  0000     0000 ";
	static char cvStrIn0[5] = "+0.0";
	static char cvStrIn1[5] = "+0.0";

    hw.display.Fill(false);

	int cursorY = 0;
    hw.display.SetCursor(0, cursorY);
	sprintf(row,"%s %s", headerRow, SelectedStep->CV0Params._waitForLatch?"*":"f");

    hw.display.WriteString(row, Font_7x10, true);
	cursorY += 12;

	static float cv = 0.0f;

	cv = SelectedStep->CV0Params.CVInNorm*5.0f;
	if (cv < 0)
		sprintf(cvStrIn0,"% 1.1f",cv);
	else
		sprintf(cvStrIn0,"%1.2f",cv);

	cv = SelectedStep->CV1Params.CVInNorm*5.0f;
	if (cv < 0)
		sprintf(cvStrIn1,"% 1.1f",cv);
	else
		sprintf(cvStrIn1, "%1.2f",cv);
	sprintf(row, "%s %s | InCV", cvStrIn0, cvStrIn1);

	hw.display.SetCursor(0, cursorY);
	hw.display.WriteString(row, Font_7x10, true);
	cursorY += 12;

	sprintf(row, " +%02d  +%02d | A:%d", SelectedStep->CV0Params.SemitoneAdd, SelectedStep->CV1Params.SemitoneAdd,UpperSelectDex + 1);
	hw.display.SetCursor(0, cursorY);
	hw.display.WriteString(row, Font_7x10, true);
	cursorY += 12;

	sprintf(row, " +%02d  +%02d | B:%d", SelectedStep->CV0Params.SemitoneAdd, SelectedStep->CV1Params.SemitoneAdd,UpperSelectDex + 1);
	hw.display.SetCursor(0, cursorY);
	hw.display.WriteString(row, Font_7x10, true);
	cursorY += 12;

	sprintf(row, "%1.2f %1.2f | OutCV", (SelectedStep->CV0Params.CVOutNorm * 5.0f), (SelectedStep->CV1Params.CVOutNorm * 5.0f));
	hw.display.SetCursor(0, cursorY);
	hw.display.WriteString(row, Font_7x10, true);
	hw.display.Update();
}

void UpdateLeds()
{
	for (size_t i = 0; i < 8; i++)
	{
		hw.led_driver.SetLed(upperKeyLeds[i],i==UpperSelectDex?1.0f:0.0f);
	}
	hw.led_driver.SwapBuffersAndTransmit();
}

void InitConstants()
{
	SetUpperSelection(0);
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
		UpdateLeds();
        System::Delay(10);
    }
}
