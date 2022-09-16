//
// Created by edall on 9/15/2022.
//
#include "daisy_field.h"
#include "AdderStepParams.h"

AdderStepParams::AdderStepParams()
{
	CV0Params.Init(0);
	CV1Params.Init(1);
}

void AdderStepParams::NotifySelectedStep(daisy::DaisyField &hw)
{
	// assumes that hw.ProcessAllControls() has happened before this is called

	// Lock in the current knob values, so we only update our semitones if the knob value changes
	// beyond a semitone worth of knob amount.
	CV0Params.NotifySelected(hw);
	CV1Params.NotifySelected( hw);
}


void AdderStepParams::ProcessStepControls(daisy::DaisyField &hw)
{
	CV0Params.ProcessKnobInput(hw);
	CV1Params.ProcessKnobInput(hw);
}

void AdderStepParams::ReadCVInput(daisy::DaisyField &hw)
{
	CV0Params.ReadCVInput(hw);
	CV1Params.ReadCVInput(hw);
}

void AdderStepParams::CalcVoltageAdd()
{
	CV0Params.CalcVoltageAdd();
	CV1Params.CalcVoltageAdd();
}

void AdderStepParams::SendCVOut(daisy::DaisyField &hw)
{
	hw.SetCvOut1((int)(CV0Params.CVOutNorm * 4095.0f));
	hw.SetCvOut2((int)(CV1Params.CVOutNorm * 4095.0f));
}
