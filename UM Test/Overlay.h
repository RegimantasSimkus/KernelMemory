#pragma once
#include <Windows.h>
#include <iostream>
#include "Overlay/directx9.h"
#include "Overlay/d3dxdraw.h"

class Overlay
{
public:
	Overlay() :
		hWnd(0),
		hTargetWnd(0),
		wc{}
	{

	}

private:
	static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		switch (msg)
		{
		case WM_CREATE:
		{
			break;
		}
		case WM_DESTROY:
		{

			PostQuitMessage(0); // request system to destroy
			break;
		}
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{
				g_d3dResizeW = (UINT)LOWORD(lParam);
				g_d3dResizeH = (UINT)HIWORD(lParam);
				break;
			}
		}
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}

public:
	bool Create()
	{
		wc = { 0 };
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.lpfnWndProc = WndProc;
		wc.style = CS_HREDRAW | CS_VREDRAW; // "Redraw if move/size changes client width" | "Redraw if move/size changes client height"
		wc.hbrBackground = WHITE_BRUSH;
		wc.hInstance = GetModuleHandleA(NULL);
		wc.hIcon = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = "km-overlay";
		
		if (!RegisterClassExA(&wc))
		{
			printf("Failed to register window class");
			return false;
		}

		RECT rect;
		GetWindowRect(hTargetWnd, &rect);

		hWnd = CreateWindowExA(
			WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
			wc.lpszClassName,
			nullptr,
			WS_POPUP, // used for ... and other temporary windows that appear outside an application's main window.
			rect.left, rect.top, 
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL,
			NULL,
			wc.hInstance,
			nullptr
		);

		if (!hWnd)
		{
			printf("Failed to create window!\n");
			return false;
		}

		SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_COLORKEY);

		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);

		if (CreateD3DDevice(hWnd))
		{
			printf("Successfully created d3d!\n");
		}

		return true;
	}

	void SetTargetProcess(HWND hWnd)
	{
		hTargetWnd = hWnd;
	}

	void Think( void(*fnRender)() )
	{
		MSG msg;
		while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (g_d3dResizeW || g_d3dResizeH)
		{
			g_d3dparams.BackBufferWidth = g_d3dResizeW;
			g_d3dparams.BackBufferHeight = g_d3dResizeH;

			g_d3dResizeW = 0;
			g_d3dResizeH = 0;
			ResetD3DDevice();
		}

		g_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(0, 0, 0, 0);	
		g_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);

		if (g_pDevice->BeginScene() >= 0)
		{

			if (fnRender)
				fnRender();

			g_pDevice->EndScene();
		}
		HRESULT result = g_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
	}

	bool Destroy()
	{
		DestroyD3DDevice();

		UnregisterClassA("km-overlay", wc.hInstance);

		DestroyWindow(hWnd);
		return true;
	}

	HWND hTargetWnd;
	HWND hWnd;
	WNDCLASSEXA wc;
};