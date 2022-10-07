//
// Created by edall on 9/15/2022.
//

#include "MidiBoxBankParams.h"
#include "daisy_field.h"
#include "daisysp.h"
#include <algorithm>
using namespace daisy;

void MidiBoxBankParams::Init(int bankDex, int channel)
{
	BankDex = bankDex;
	Channel = channel;
	CCStartRange = 10 + bankDex * 8;
	for (int i = 0; i < 8; i++)
	{
		LastKnobVal[i] = 0.0f;
		CurKnobVal[i] = 0.0f;
		CCValues[i] = 0;
		CCNumbers[i] = CCStartRange + i;
		HaveNewValue[i] = false;
		_waitForLatch[i] = false;
	}
}

void MidiBoxBankParams::NotifySelected(DaisyField &hw)
{
	for (int i = 0; i < 8; i++)
	{
		LastKnobVal[i] = hw.GetKnobValue(i);
		_waitForLatch[i] = true;
	}
}

void MidiBoxBankParams::ProcessKnobInput(DaisyField &hw)
{
	static float newKnobVal = 0.0f;
	for (int i = 0; i < 8; i++)
	{
		newKnobVal = hw.GetKnobValue(i);
		float delta = newKnobVal - LastKnobVal[i];
		if (_waitForLatch[i] && (abs(delta) > 0.1))
		{
			_waitForLatch[i] = false;
		}
		if (!_waitForLatch[i] && (abs(delta) > 0.1))
		{
			CurKnobVal[i] = newKnobVal;
			LastKnobVal[i] = newKnobVal;
			CCValues[i] = (newKnobVal * 127.0f);
			HaveNewValue[i] = true;
		}
	}
}

void MidiBoxBankParams::SendCCValues(DaisyField &hw)
{
	for (int i = 0; i < 8; i++)
	{
		uint8_t data[3] = { 0 };
		if (HaveNewValue[i])
		{
			//TODO - send the actual midi
			data[0] = ((uint8_t)(Channel) & 0x0F) + 0xB0;  // limit channel byte, add status byte
			data[1] = CCNumbers[i] & 0x7F;
			data[2] = CCValues[i] & 0x7F;
			hw.midi.SendMessage(data,3);

			HaveNewValue[i] = false;
		}
	}
}
