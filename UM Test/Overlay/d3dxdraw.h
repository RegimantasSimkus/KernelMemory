#pragma once

namespace draw
{
	bool DrawTextA(PCHAR text, FLOAT x, FLOAT y, D3DCOLOR color = 0xFFFFFFFF, DWORD fmt = 0);
	bool DrawTextAOutlined(PCHAR text, FLOAT x, FLOAT y, INT thickness = 1, D3DCOLOR color = 0xFFFFFFFF, DWORD fmt = 0);

	bool DrawLine(FLOAT x0, FLOAT y0, FLOAT x1, FLOAT y1, D3DCOLOR color = 0xFFFFFFFF);
}