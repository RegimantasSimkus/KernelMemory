#include <iostream>

#include "CSMemory.h"
#include "MemoryObject.h"

void CSMemory::LoadModule(const WCHAR* moduleName, ULONG_PTR& baseOut)
{
	printf("[-] Loading %ls...\n", moduleName);
	while ((baseOut = RequestModuleHandle(moduleName)) == 0)
		Sleep(50);
	printf("\t%ls -> %X\n", moduleName, (DWORD)baseOut);
}

BOOL WINAPI CSMemory::_EnumWind(HWND hWnd, LPARAM lParam)
{
	_EnumWindParam* param = (_EnumWindParam*)lParam;

	DWORD dwProcId;
	GetWindowThreadProcessId(hWnd, &dwProcId);
	if (param->ProcId == dwProcId)
	{
		param->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

HWND CSMemory::LoadHWND()
{
	_EnumWindParam EnumWindParam;
	EnumWindParam.hWnd = 0;
	EnumWindParam.ProcId = (DWORD)ProcessId;

	EnumWindows(_EnumWind, (LPARAM)(&EnumWindParam));

	this->hWnd = EnumWindParam.hWnd;
	return this->hWnd;
}

bool CSMemory::Initialize()
{
	printf("[Initializing CS2 Memory Module]\n");

	Open(R"(\\.\KernelMemory)");
	if (!IsOpen())
	{
		printf("DRIVER IS NOT RUNNING\n");
		return false;
	}

	while ((ProcessId = RequestProcessId()) == 0)
		Sleep(50);
	printf("ProcessID -> %X\n", (DWORD)ProcessId);


	if (!LoadHWND())
	{
		printf("Failed to load hWnd\n");
		return false;
	}
	printf("HWND -> %X\n", (DWORD)hWnd);

	LoadModule(L"client.dll", ClientDll);
	return true;
}
