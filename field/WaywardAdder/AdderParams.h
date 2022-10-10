//
// Created by edall on 9/15/2022.
//

#ifndef WAYWARDDAISYFORK_ADDERPARAMS_H
#define WAYWARDDAISYFORK_ADDERPARAMS_H

#include "daisy_field.h"

class AdderParams
{
public:
	int CVId;
	float KnobVal;
	float LastKnobVal;
	float CVInNorm;
	float CVOutNorm;
	int SemitoneAdd;
	float SemitoneRange;
	bool _waitForLatch;
	float SlewAmtNorm;
	float GateProbNorm;
public:
	void Init(int CVId);
	void NotifySelected(daisy::DaisyField &hw);
	void ProcessKnobInput(daisy::DaisyField &hw);
	void ReadCVInput(daisy::DaisyField &hw);
	float CalcVoltageAdd();
};

#endif //WAYWARDDAISYFORK_ADDERPARAMS_H
