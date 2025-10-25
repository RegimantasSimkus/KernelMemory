#include "directx9.h"
#include "d3dxdraw.h"

bool draw::DrawLine(FLOAT x0, FLOAT y0, FLOAT x1, FLOAT y1, D3DCOLOR color)
{
	if (SUCCEEDED(g_pLine->Begin()))
	{
		D3DXVECTOR2 vertices[] = {
			{x0, y0},
			{x1, y1}
		};

		g_pLine->SetWidth(2);
		g_pLine->Draw(vertices, 2, color);

		g_pLine->End();
		return true;
	}
	return false;
}

bool draw::DrawTextA(PCHAR text, FLOAT x, FLOAT y, D3DCOLOR color, DWORD fmt)
{
	RECT rect;
	rect.left = rect.right = x;
	rect.top = rect.bottom = y;

	g_pFont->DrawTextA(nullptr, text, -1, &rect, DT_NOCLIP | fmt, color);
	return true;
}

bool draw::DrawTextAOutlined(PCHAR text, FLOAT x, FLOAT y, INT thickness, D3DCOLOR color, DWORD fmt)
{
	for (int _x = -thickness; _x <= thickness; _x++)
	{
		for (int _y = -thickness; _y <= thickness; _y++)
		{
			if (_x == _y)
				continue;
			DrawTextA(text, x + _x, y + _y, D3DCOLOR_RGBA(8, 8, 8, 255), fmt);
		}
	}
	DrawTextA(text, x, y, color, fmt);
	return true;
}