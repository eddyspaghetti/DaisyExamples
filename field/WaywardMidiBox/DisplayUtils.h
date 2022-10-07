#pragma once
#include "daisy_field.h"

using namespace daisy;


class DisplayUtils
{
	static DaisyField* s_hw;

	public:
	static void InitDisplay(DaisyField *hw) { s_hw = hw; }

	static void Clear();
	static void DrawVBar(int x, int y, int width, int height, float valNormalized);
	static void DrawEnvelope(int envMinX, int envMinY, int envMaxX, int envMaxY, float aPct, float dPct, float rPct, float sLevel);
};