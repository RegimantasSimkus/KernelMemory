#include "Vector3.h"
#include "../CSFrameData.h"
#include <Windows.h>

Vector3 Vector3::ToScreen()
{
	float cx = FrameData.ScrW * .5f;
	float cy = FrameData.ScrH * .5f;

	float w = FrameData.ViewMatrix[3][0] * x + FrameData.ViewMatrix[3][1] * y + FrameData.ViewMatrix[3][2] * z + FrameData.ViewMatrix[3][3];

	float X = cx + (FrameData.ViewMatrix[0][0] * x + FrameData.ViewMatrix[0][1] * y + FrameData.ViewMatrix[0][2] * z + FrameData.ViewMatrix[0][3]) / w * cx;
	float Y = cy - (FrameData.ViewMatrix[1][0] * x + FrameData.ViewMatrix[1][1] * y + FrameData.ViewMatrix[1][2] * z + FrameData.ViewMatrix[1][3]) / w * cy;
	return { X, Y, w };
}