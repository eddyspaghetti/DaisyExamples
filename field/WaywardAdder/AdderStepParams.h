//
// Created by edall on 9/15/2022.
//

#ifndef WAYWARDDAISYFORK_ADDERSTEPPARAMS_H
#define WAYWARDDAISYFORK_ADDERSTEPPARAMS_H

#include "AdderParams.h"

class AdderStepParams
{
public:
	int StepId = 0;
	AdderParams CVParamsA;
	AdderParams CVParamsB;
public:
	explicit AdderStepParams();

	void Init(int stepId);

	void NotifySelectedStep(daisy::DaisyField &hw);

	void ProcessStepControls(daisy::DaisyField &hw);

	void ReadCVInput(daisy::DaisyField &hw);

	void CalcVoltageAdd();

	void SendCVOut(daisy::DaisyField &hw);
};

#endif //WAYWARDDAISYFORK_ADDERSTEPPARAMS_H
