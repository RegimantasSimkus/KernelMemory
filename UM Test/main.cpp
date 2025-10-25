#include <Windows.h>
#include <iostream>
#include "KernelMemory.h"
#include "Overlay.h"
#include "CS2/CSGame.h"
#include "CS2/CSFrameData.h"

inline CS2 Game;

void Render()
{
	FrameData.CaptureFrame();

	CSPawn localPlayer = CS2::GetLocalPlayerPawn();
	int localTeam = localPlayer.GetTeam();
	
	Vector3 localpos = localPlayer.GetOrigin();
	Vector3 screenPos = localpos.ToScreen();

	for (int i = 0; i < 64; i++)
	{
		CSPawn player = CS2::GetPawnIndex(i);
		if (!player)
			continue;

		if (player == localPlayer || player.GetTeam() == localTeam)
			continue;

		if (player.GetHealth() <= 0)
			continue;

		Vector3 playerpos = player.GetOrigin();
		Vector3 screenpos = playerpos.ToScreen();
		if (screenpos.z < 0.01f)
			continue;

		float distanceSqr = playerpos.DistanceSqr(localpos);
		float ratio = (distanceSqr / (1700 * 1700));
		if (ratio > 1)
			ratio = 1;

		D3DCOLOR col = D3DCOLOR_RGBA(
			(int)(ratio * 255.f),
			(int)((1 - ratio) * 255.f),
			0,
			255
		);

		draw::DrawLine(FrameData.ScrW / 2, FrameData.ScrH, screenpos.x, screenpos.y, col);

		char text[10];
		sprintf_s(text, "HP: %d", player.GetHealth());

		draw::DrawTextAOutlined(text, screenpos.x, screenpos.y, 1, D3DCOLOR_RGBA(255, 255, 255, 255), DT_CENTER);
	}
}

void main()
{
	if (!Memory.Initialize())
	{
		printf("Failed to open...\n");
		return;
	}

	Overlay overlay;
	overlay.SetTargetProcess(Memory.hWnd);
	if (overlay.Create())
	{
		printf("Successfully created!\n");
	}

	FrameData.ScrW = GetSystemMetrics(SM_CXSCREEN);
	FrameData.ScrH = GetSystemMetrics(SM_CYSCREEN);

	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
			break;

		overlay.Think(Render);
	}

	overlay.Destroy();
}