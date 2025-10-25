#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

extern IDirect3D9* g_D3D;
extern IDirect3DDevice9* g_pDevice;
extern D3DPRESENT_PARAMETERS g_d3dparams;

extern ID3DXLine* g_pLine;
extern ID3DXFont* g_pFont;

bool CreateD3DDevice(HWND hWnd);

bool DestroyD3DDevice();

extern UINT g_d3dResizeW, g_d3dResizeH;

bool ResetD3DDevice();