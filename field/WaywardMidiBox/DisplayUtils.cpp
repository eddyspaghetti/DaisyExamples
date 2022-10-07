#include "DisplayUtils.h"

using namespace daisy;

DaisyField *DisplayUtils::s_hw;

void DisplayUtils::Clear()
{
	s_hw->display.Fill(false);
}

// static
void DisplayUtils::DrawVBar(int x, int y, int width, int height, float valNormalized)
{
	int baseY = y;
	int topY = y - (int) (valNormalized * (float) height);
	for (int i = 0; i < width; i++)
	{
		s_hw->display.DrawLine(x+i,baseY,x+i,topY,true);
	}
}

// static
void DisplayUtils::DrawEnvelope(int envMinX, int envMinY, int envMaxX, int envMaxY, float aPct, float dPct, float rPct, float sLevel)
{
	float sPct = fmin(.1,1.0f-aPct-dPct-rPct);
	float totalWidth = (envMaxX-envMinX);

	int x = envMinX;
	int sustainY = envMaxY - (int) ((float)sLevel * (float)(envMaxY-envMinY));

	int endX = x + (int) (aPct * totalWidth);
	s_hw->display.DrawLine(x,envMaxY,endX, envMinY, true);

	x = endX;
	endX = x + (int) (dPct * totalWidth);
	s_hw->display.DrawLine(x,envMinY,endX, sustainY, true);

	x = endX;
	endX = x + (int) (sPct * totalWidth);
	s_hw->display.DrawLine(x,sustainY,endX, sustainY, true);

	x = endX;
	endX = x + (int) (rPct * totalWidth);
	s_hw->display.DrawLine(x,sustainY,endX, envMaxY, true);
	
	s_hw->display.SetCursor(envMinX,envMaxY + 5);
	s_hw->display.WriteString("ADSR",Font_6x8,true);
}
