#pragma once
#include "Engine/ViewMatrix.h"

class CSFrameData
{
public:
	void CaptureFrame();

	ViewMatrix ViewMatrix;
	int ScrW;
	int ScrH;
};

inline CSFrameData FrameData;