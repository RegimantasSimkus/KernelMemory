#pragma once
#include <ntifs.h>
#include <wdf.h>

NTSTATUS DriverEntry(
	PDRIVER_OBJECT DriverObject,
	PUNICODE_STRING RegistryPath
);

VOID DriverUnload(
	PDRIVER_OBJECT DriverObject
);
