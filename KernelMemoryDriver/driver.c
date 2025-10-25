#include "driver.h"
#include "events.h"
#include "routines.h"
#include "process.h"

UNICODE_STRING g_SymName;
PDEVICE_OBJECT g_DeviceObject;

NTSTATUS DriverEntry(
	PDRIVER_OBJECT DriverObject,
	PUNICODE_STRING RegistryPath
)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	
	NTSTATUS status;
	UNICODE_STRING DeviceName;
	RtlInitUnicodeString(&DeviceName, L"\\Device\\KernelMemory");
	RtlInitUnicodeString(&g_SymName, L"\\DosDevices\\KernelMemory");

	DbgPrintEx(0, 0, "Driver entry!");

	status = IoCreateDevice(
		DriverObject,
		0,
		&DeviceName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&g_DeviceObject
	);
	if (!NT_SUCCESS(status))
	{
		DbgPrintEx(0, 0, "Failed to create device...\n");
		return status;
	}

	IoCreateSymbolicLink(&g_SymName, &DeviceName);
	DriverObject->DriverUnload = DriverUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = EvtCreateFile;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = EvtCloseFile;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = EvtIoDeviceControl;

	//ExInitializeFastMutex(&CS2DataMutex);
	DbgPrintEx(0, 0, "Init data...\n");

	InitializeProcessData();
	PsSetLoadImageNotifyRoutine(ProcessNotifyRoutine);

	STATUS_INVALID_PARAMETER;
	return status;
}

VOID DriverUnload(
	PDRIVER_OBJECT DriverObject
)
{
	UNREFERENCED_PARAMETER(DriverObject);

	PsRemoveLoadImageNotifyRoutine(ProcessNotifyRoutine);

	IoDeleteSymbolicLink(&g_SymName);
	IoDeleteDevice(g_DeviceObject);

	DbgPrintEx(0, 0, "Driver unloaded");
}