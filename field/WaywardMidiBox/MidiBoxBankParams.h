//
// Created by edall on 9/15/2022.
//

#ifndef WAYWARDDAISYFORK_ADDERPARAMS_H
#define WAYWARDDAISYFORK_ADDERPARAMS_H

#include "daisy_field.h"

class MidiBoxBankParams
{
public:
	int BankDex;
	int Channel;
	int CCStartRange;
	int CCValues[8];
	int CCNumbers[8];
	float LastKnobVal[8];
	float CurKnobVal[8];
	bool HaveNewValue[8];
	bool _waitForLatch[8];
public:
	void Init(int bankDex, int channel);
	void NotifySelected(daisy::DaisyField &hw);
	void ProcessKnobInput(daisy::DaisyField &hw);
	void SendCCValues(daisy::DaisyField &hw);
};

#endif //WAYWARDDAISYFORK_ADDERPARAMS_H
