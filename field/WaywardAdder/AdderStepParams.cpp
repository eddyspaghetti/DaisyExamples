//
// Created by edall on 9/15/2022.
//
#include "daisy_field.h"
#include "AdderStepParams.h"

AdderStepParams::AdderStepParams()
{
	StepId = 0;
	CVParamsA.Init(0);
	CVParamsB.Init(1);
}

void AdderStepParams::Init(int stepId)
{
	StepId = stepId;
}


void AdderStepParams::NotifySelectedStep(daisy::DaisyField &hw)
{
	// assumes that hw.ProcessAllControls() has happened before this is called

	// Lock in the current knob values, so we only update our semitones if the knob value changes
	// beyond a semitone worth of knob amount.
	CVParamsA.NotifySelected(hw);
	CVParamsB.NotifySelected(hw);
}


void AdderStepParams::ProcessStepControls(daisy::DaisyField &hw)
{
	CVParamsA.ProcessKnobInput(hw);
	CVParamsB.ProcessKnobInput(hw);
}

void AdderStepParams::ReadCVInput(daisy::DaisyField &hw)
{
	CVParamsA.ReadCVInput(hw);
	CVParamsB.ReadCVInput(hw);
}

void AdderStepParams::CalcVoltageAdd()
{
	CVParamsA.CalcVoltageAdd();
	CVParamsB.CalcVoltageAdd();
}

void AdderStepParams::SendCVOut(daisy::DaisyField &hw)
{
	hw.SetCvOut1((int)(CVParamsA.CVOutNorm * 4095.0f));
	hw.SetCvOut2((int)(CVParamsB.CVOutNorm * 4095.0f));
}
