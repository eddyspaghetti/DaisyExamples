//
// Created by edall on 10/9/2022.
//

#include "../../libDaisy/src/daisy_field.h"
#include "AdderStepParams.h"
#include "AdderDisplay.h"

using namespace daisy;

void AdderDisplay::Init(DaisyField *hw)
{
	_hw = hw;
}

void AdderDisplay::ShowSelectedStep(AdderStepParams *selectedStep)
{
	static const char titleRow[20] = "Adder v0.1";
	static const char headerRow[20] = "Off Slw Rng P/L";
	static char rowA[20]   = "A +02 100  20 40%";
	static char rowB[20]   = "B +02 100  20 Lnk";
	static char rowC[20]   = "Gate          Major";

	// clear screen
	_hw->display.Fill(false);

	// draw title
	_hw->display.DrawRect(0,0,127,10, true, true);
	_hw->display.SetCursor(36,2);
	_hw->display.WriteString(titleRow, Font_6x8, false);

	// draw header
	_hw->display.SetCursor(18,13);
	_hw->display.WriteString(headerRow, Font_6x8, true);

	// draw row A
	_hw->display.SetCursor(4,26);
	sprintf(rowA,"A +%02d %03d  %02d %03d",
	        selectedStep->CVParamsA.SemitoneAdd,
	        (int)(selectedStep->CVParamsA.SlewAmtNorm * 100.0f),
	        (int) selectedStep->CVParamsA.SemitoneRange,
	        (int)(selectedStep->CVParamsA.GateProbNorm * 100.0f));

	_hw->display.WriteString(rowA, Font_6x8, true);

	// draw row B
	sprintf(rowB,"B +%02d %03d  %02d %03d",
	        selectedStep->CVParamsB.SemitoneAdd,
	        (int)(selectedStep->CVParamsB.SlewAmtNorm * 100.0f),
	        (int) selectedStep->CVParamsB.SemitoneRange,
	        (int)(selectedStep->CVParamsB.GateProbNorm * 100.0f));
	_hw->display.SetCursor(4,40);
	_hw->display.WriteString(rowB, Font_6x8, true);

	// separating line
	_hw->display.DrawLine(0,52,127,52,true);

	// draw row c
	_hw->display.SetCursor(4,55);
	_hw->display.WriteString(rowC, Font_6x8, true);


	_hw->display.SetCursor(115,13);
	_hw->display.DrawRect(112,10,127,30,true,true);
	char c = '0' + (char)selectedStep->StepId;
	_hw->display.WriteChar(c, Font_11x18, false);

	_hw->display.DrawRect(112,30,127,52,true,false);
	_hw->display.SetCursor(115,33);


	// draw upcoming step queued (if needed
	c = '0' + (char)selectedStep->StepId + 1;
	_hw->display.WriteChar(c, Font_11x18, true);

	_hw->display.Update();
}

