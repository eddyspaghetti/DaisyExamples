//
// Created by edall on 9/15/2022.
//

#include "AdderParams.h"
#include "daisysp.h"
#include <algorithm>

void AdderParams::Init(int CVId)
{
	this->CVId = CVId;
	SemitoneAdd = 0;
	KnobVal = 0.0f;
	LastKnobVal = 0.0f;
	CVInNorm = 0.0f;
	CVOutNorm = 0.0f;
	SemitoneRange = 24.0f;
	SlewAmtNorm = 0.0f;
	_waitForLatch = false;
}

void AdderParams::NotifySelected(daisy::DaisyField &hw)
{
	LastKnobVal = hw.GetKnobValue(CVId);
	_waitForLatch = true;
}

void AdderParams::ReadCVInput(daisy::DaisyField &hw)
{
	CVInNorm = hw.GetCvValue(CVId);
}

void AdderParams::ProcessKnobInput(daisy::DaisyField &hw)
{
	float newKnobVal = hw.GetKnobValue(CVId);
	float delta = newKnobVal - LastKnobVal;
	if (_waitForLatch && (abs(delta) > 0.1f))
	{
		_waitForLatch = false;
	}

	if (!_waitForLatch)
	{
		KnobVal = newKnobVal;
		LastKnobVal = newKnobVal;
	}
}

float AdderParams::CalcVoltageAdd()
{
	// convert knob value to semitones, spread over the octave range.
	static int semitones = 0.0f;
	static float voltsToAdd = 0.0f;

	// quantize our input to semitones by casting to int.
	semitones = (int) (KnobVal * (SemitoneRange+1));
	SemitoneAdd = abs(semitones); // store this, so we can display it

	// turn that back into voltage - 1/12v per semitone
	// however, our input is normalized with 0->1 => 0->5V, so we need to map onto that range to add them.
	voltsToAdd = ((float)semitones) / 12.0f / 5.0f;

	// now we're in the same normalized (0-1) space, so add voltages.
	// cache that value so we can display it.
	CVOutNorm = daisysp::fclamp(CVInNorm + voltsToAdd,0.0f,1.0f);
	return CVOutNorm;
}

