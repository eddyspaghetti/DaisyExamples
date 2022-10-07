#include "daisy_field.h"
#include "daisysp.h"
//#include "AdderStepParams.h"
#include <array>
#include <algorithm>
#include "DisplayUtils.h"
#include "MidiBoxBankParams.h"

using namespace daisy;
using namespace daisysp;

DaisyField hw;

//AdderStepParams UpperRowParams[8];
//AdderStepParams *SelectedStep;

MidiBoxBankParams BankParams[8];
MidiBoxBankParams *SelectedBank;
int GlobalMidiChannel = 1;

size_t UpperSelectDex = 0;
bool message = false;

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
//	SelectedStep->ReadCVInput(hw);
}

void SetUpperSelection(size_t newDex)
{
	UpperSelectDex = newDex;
	SelectedBank = &(BankParams[UpperSelectDex]);
	SelectedBank->NotifySelected(hw);
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

	SelectedBank->ProcessKnobInput(hw);
}

void AddKnobCV()
{
//	SelectedStep->CalcVoltageAdd();
}

void WriteCV()
{
//	SelectedStep->SendCVOut(hw);
}

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    Controls();
	SelectedBank->SendCCValues(hw);
//    ReadCV();
//    AddKnobCV();
//    WriteCV();

    for(size_t i = 0; i < size; i++)
    {
//         Zero samples prior to summing

        out[0][i] = 0.f;
        out[1][i] = 0.f;
    }
}

void UpdateDisplay()
{
	static char row[20] = "  0000     0000 ";
	static char ccStr0[5] = "000";

    DisplayUtils::Clear();

	int cursorY = 0;
    hw.display.SetCursor(0, cursorY);

	static int spacing = 16;
	static int textX = 2;
	static int textY = 33;
	static int textUnderlineY = textY + 8;
	int CC;
	bool showLine = false;
	for (int i = 0; i < 8; i++)
	{
		CC = SelectedBank->CCStartRange + i;
		showLine = CC >= 100;
		if (showLine)
			CC -= 100;

		sprintf(ccStr0, "%02d\0",CC);
		textX = 2 + i*spacing;
		hw.display.SetCursor(2 + i * spacing, 34);
		hw.display.WriteString(ccStr0, Font_6x8, true);

		if (showLine)
			hw.display.DrawLine(textX,textUnderlineY, textX + 10, textUnderlineY, true);

		DisplayUtils::DrawVBar(5 + i*spacing,30,5,30,SelectedBank->CurKnobVal[i]);
	}

	sprintf(row,"A%d",SelectedBank->BankDex);
	if (message)
	{
		sprintf(row,"**\0");
		message = false;
	}
	hw.display.SetCursor(2, 46);
	hw.display.WriteString(row, Font_11x18, true);

	sprintf(row,"Ch:%02d   LATCH",SelectedBank->Channel);
	hw.display.SetCursor( 36, 50 );
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
	for(int i = 0; i < 8; i++)
	{
		BankParams[i].Init(i+1, GlobalMidiChannel);
	}
	SetUpperSelection(0);
}


int main(void)
{
    hw.Init(); // Don't try to use SDRAM until after this Init function
	DisplayUtils::InitDisplay(&hw);
    InitConstants();
    hw.StartAdc();
    hw.StartAudio(AudioCallback);
	hw.midi.StartReceive();
    while(true)
    {
	    hw.midi.Listen();
	    // Handle MIDI Events
	    while(hw.midi.HasEvents())
	    {
		    hw.midi.PopEvent();
			message = true;
	    }

        UpdateDisplay();
		UpdateLeds();
        System::Delay(10);
    }
}
