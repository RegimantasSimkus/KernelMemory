#pragma once

struct ViewMatrix
{
	float* operator[](int i)
	{
		return m[i];
	}

	float m[4][4];
};