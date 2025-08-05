#pragma once
#include <ntifs.h>

NTSTATUS EvtCreateFile(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
);

NTSTATUS EvtCloseFile(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
);

NTSTATUS EvtIoDeviceControl(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
);
