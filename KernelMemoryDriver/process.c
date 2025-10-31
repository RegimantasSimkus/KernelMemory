#include "process.h"

VOID SetTargetProcessId(ULONG_PTR ProcId)
{
	KIRQL oldIrql;
	KeAcquireSpinLock(&g_ProcessDataMutex, &oldIrql);

	g_ProcessId = ProcId;

	KeReleaseSpinLock(&g_ProcessDataMutex, oldIrql);
}

ULONG_PTR GetTargetProcessId()
{
	ULONG_PTR ProcId = 0;

	KIRQL oldIrql;
	KeAcquireSpinLock(&g_ProcessDataMutex, &oldIrql);

	ProcId = g_ProcessId;

	KeReleaseSpinLock(&g_ProcessDataMutex, oldIrql);
	return ProcId;
}

RTL_GENERIC_COMPARE_RESULTS LoadedProcessModulesTableCompareRoutine(
	PRTL_GENERIC_TABLE Table,
	PVOID FirstStruct,
	PVOID SecondStruct
)
{
	UNREFERENCED_PARAMETER(Table);

	PLOADED_PROCESS_MODULE First = (PLOADED_PROCESS_MODULE)FirstStruct;
	PLOADED_PROCESS_MODULE Second = (PLOADED_PROCESS_MODULE)SecondStruct;

	// Might as well be insensitive, doesnt really matter client.dll vs Client.DLL or whatever
	LONG cmp = wcscmp(First->ModuleName, Second->ModuleName);
	if (cmp > 0) 
		return GenericGreaterThan;
	if (cmp < 0)
		return GenericLessThan;
	return GenericEqual;
}

PVOID LoadedProcessModulesTableAllocateRoutine(
	PRTL_GENERIC_TABLE Table,
	CLONG Bytes
)
{
	UNREFERENCED_PARAMETER(Table);

	// ExAllocatePool2 allocates zero-initialized memory
	PVOID alloc = ExAllocatePool2(
		POOL_FLAG_NON_PAGED,
		Bytes,
		'sckm' // kmcs - Kernel Memory CS
	);

	return alloc;
}

VOID NTAPI LoadedProcessModulesTableFreeRouine(
	PRTL_GENERIC_TABLE Table,
	PVOID Buffer
)
{
	UNREFERENCED_PARAMETER(Table);
	
	ExFreePool2(Buffer, 'sckm', NULL, 0);
}

NTSTATUS InitializeProcessData()
{
	KeInitializeSpinLock(&g_ProcessDataMutex);

	RtlInitializeGenericTable(&g_ModuleList, LoadedProcessModulesTableCompareRoutine, LoadedProcessModulesTableAllocateRoutine, LoadedProcessModulesTableFreeRouine, NULL);
	return STATUS_SUCCESS;
}

VOID InsertModuleToListEx(PWCHAR ModuleName, ULONG_PTR ImageBase)
{
	LOADED_PROCESS_MODULE Entry;
	Entry.ImageBase = ImageBase;
	wcscpy_s(Entry.ModuleName, sizeof(Entry.ModuleName) / sizeof(unsigned short), ModuleName);

	RtlInsertElementGenericTable(&g_ModuleList, &Entry, sizeof(Entry), NULL);
}

VOID InsertModuleToList(PWCHAR ModuleName, ULONG_PTR ImageBase)
{
	PLOADED_PROCESS_MODULE ModuleEntry = GetModuleFromList(ModuleName);
	if (ModuleEntry)
	{
		ModuleEntry->ImageBase = ImageBase;
		return;
	}
		
	KIRQL oldIrql;
	KeAcquireSpinLock(&g_ProcessDataMutex, &oldIrql);

	InsertModuleToListEx(ModuleName, ImageBase);

	KeReleaseSpinLock(&g_ProcessDataMutex, oldIrql);
}

PLOADED_PROCESS_MODULE GetModuleFromList(PWCHAR ModuleName)
{
	KIRQL oldIrql;
	KeAcquireSpinLock(&g_ProcessDataMutex, &oldIrql);

	PLOADED_PROCESS_MODULE Found = NULL;
	PVOID RestartKey = NULL;
	for (PLOADED_PROCESS_MODULE entry = (PLOADED_PROCESS_MODULE)RtlEnumerateGenericTableWithoutSplaying(&g_ModuleList, &RestartKey);
		entry != NULL;
		entry = (PLOADED_PROCESS_MODULE)RtlEnumerateGenericTableWithoutSplaying(&g_ModuleList, &RestartKey))
	{
		if (!wcscmp(entry->ModuleName, ModuleName))
		{
			Found = entry;
			break;
		}
	}

	KeReleaseSpinLock(&g_ProcessDataMutex, oldIrql);
	return Found;
}

NTSTATUS ReleaseProcessData()
{
	PLOADED_PROCESS_MODULE entry;

	KIRQL oldIrql;
	KeAcquireSpinLock(&g_ProcessDataMutex, &oldIrql);

	PVOID RestartKey = NULL;
	for (entry = (PLOADED_PROCESS_MODULE)RtlEnumerateGenericTableWithoutSplaying(&g_ModuleList, &RestartKey);
		entry != NULL;
		entry = (PLOADED_PROCESS_MODULE)RtlEnumerateGenericTableWithoutSplaying(&g_ModuleList, &RestartKey))
	{
		RtlDeleteElementGenericTable(&g_ModuleList, entry);
	}

	KeReleaseSpinLock(&g_ProcessDataMutex, oldIrql);
	return STATUS_SUCCESS;
}