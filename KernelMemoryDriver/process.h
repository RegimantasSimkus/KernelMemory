#pragma once
#include <ntifs.h>

typedef struct _LOADED_PROCESS_MODULE {
	WCHAR ModuleName[32];
	ULONG_PTR ImageBase;
} LOADED_PROCESS_MODULE, *PLOADED_PROCESS_MODULE;

KSPIN_LOCK g_ProcessDataMutex;

// Loaded modules
RTL_GENERIC_TABLE g_ModuleList;

// Global constants
ULONG_PTR g_ProcessId;

NTSTATUS InitializeProcessData();

VOID InsertModuleToListEx(PWCHAR ModuleName, ULONG_PTR ImageBase);

// Wrapper using mutex
VOID InsertModuleToList(PWCHAR ModuleName, ULONG_PTR ImageBase);

PLOADED_PROCESS_MODULE GetModuleFromList(PWCHAR ModuleName);

NTSTATUS ReleaseProcessData();

VOID SetTargetProcessId(ULONG_PTR ProcId);

ULONG_PTR GetTargetProcessId();
