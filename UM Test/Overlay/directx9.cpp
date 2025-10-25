#include "directx9.h"
#include <iostream>

IDirect3D9* g_D3D = nullptr;
IDirect3DDevice9* g_pDevice = nullptr;
D3DPRESENT_PARAMETERS g_d3dparams = {};

ID3DXLine* g_pLine = nullptr;
ID3DXFont* g_pFont = nullptr;

UINT g_d3dResizeW = 0, g_d3dResizeH = 0;

bool CreateD3DDevice(HWND hWnd)
{
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!g_D3D)
		return false;

	ZeroMemory(&g_d3dparams, sizeof(g_d3dparams));
	g_d3dparams.Windowed = TRUE;
	g_d3dparams.hDeviceWindow = hWnd;
	g_d3dparams.BackBufferFormat = D3DFMT_A8R8G8B8;
	g_d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dparams.EnableAutoDepthStencil = TRUE;
	g_d3dparams.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dparams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	g_d3dparams.BackBufferWidth = 512;
	g_d3dparams.BackBufferHeight = 512;


	if (FAILED(g_D3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&g_d3dparams,
		&g_pDevice
	)))
	{
		printf("Failed to create d3d\n");
		g_D3D->Release();
		return false;
	}

	D3DXCreateLine(g_pDevice, &g_pLine);
	D3DXCreateFontW(g_pDevice, 14, 0, FW_REGULAR, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, L"Consolas", &g_pFont);
	return true;
}

bool DestroyD3DDevice()
{
	if (g_D3D)
		g_D3D->Release(), g_D3D = nullptr;

	if (g_pDevice)
		g_pDevice->Release(), g_pDevice = nullptr;

	if (g_pLine)
		g_pLine->Release(), g_pLine = nullptr;

	if (g_pFont)
		g_pFont->Release(), g_pFont = nullptr;

	return true;
}

bool ResetD3DDevice()
{
	g_pDevice->Reset(&g_d3dparams);
	return true;
}