//
// Created by edall on 10/9/2022.
//

#ifndef WAYWARDDAISYFORK_ADDERDISPLAY_H
#define WAYWARDDAISYFORK_ADDERDISPLAY_H

#include "daisy_field.h"
using namespace daisy;

class AdderDisplay
{
public:
	void Init(DaisyField *hw);
	void ShowSelectedStep(AdderStepParams *selectedStep);
private:
	DaisyField *_hw;
};


#endif //WAYWARDDAISYFORK_ADDERDISPLAY_H
