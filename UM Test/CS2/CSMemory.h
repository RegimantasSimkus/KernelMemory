#pragma once
#include "../KernelMemory.h"

class MemoryObject;

class CSMemory : public KernelMemory
{
public:
	CSMemory() :
		ProcessId(NULL),
		hWnd(NULL),
		ClientDll(NULL)
	{

	}
	
private:
	void LoadModule(const WCHAR* moduleName, ULONG_PTR& baseOut);
	struct _EnumWindParam
	{
		DWORD ProcId;
		HWND hWnd;
	};

	static BOOL WINAPI _EnumWind(HWND hWnd, LPARAM lParam);

	HWND LoadHWND();

public:
	bool Initialize();

	template <typename T>
	bool ReadMemory(ULONG_PTR address, T* value)
	{
		return ReadVirtualMemory<T>(ProcessId, address, value);
	}

	template <typename T>
	T ReadMemory(ULONG_PTR address)
	{
		T val;
		ReadVirtualMemory<T>(ProcessId, address, &val);
		return val;
	}

	HWND hWnd;
	ULONG_PTR ProcessId;
	ULONG_PTR ClientDll;
};

inline CSMemory Memory;