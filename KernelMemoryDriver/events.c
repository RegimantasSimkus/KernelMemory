#include "events.h"
#include "process.h"
#include <iocodes.h>
#include "memory.h"

NTSTATUS EvtCreateFile(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	DbgPrintEx(0, 0, "EvtCreateFile!");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS EvtCloseFile(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	DbgPrintEx(0, 0, "EvtCloseFile!");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS EvtIoDeviceControl(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	ULONG IoControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
	PVOID SystemBuffer = Irp->AssociatedIrp.SystemBuffer;

	NTSTATUS status = STATUS_SUCCESS;
	ULONG IoInfo = 0;
	switch (IoControlCode)
	{
	case IO_CTL_GETPROCESSID:
	{
		PIO_REQUEST_PROCID IoRequest = (PIO_REQUEST_PROCID)SystemBuffer;
		IoInfo = sizeof(*IoRequest);

		IoRequest->ProcessId = GetTargetProcessId();
		break;
	}
	case IO_CTL_GETMODULEHANDLE:
	{
		PIO_REQUEST_MODULE_HANDLE IoRequest = (PIO_REQUEST_MODULE_HANDLE)SystemBuffer;
		IoInfo = sizeof(*IoRequest);


		PLOADED_PROCESS_MODULE Module = GetModuleFromList(IoRequest->ModuleName);
		if (Module)
			IoRequest->ImageBase = Module->ImageBase;
		else
			IoRequest->ImageBase = 0x00;

		break;
	}
	case IO_CTL_READMEMORY:
	{
		PIO_REQUEST_READ_MEMORY IoRequest = (PIO_REQUEST_READ_MEMORY)SystemBuffer;
		IoInfo = sizeof(*IoRequest);

		NTSTATUS ReadStatus = KernelReadVirtualMemory(
			(HANDLE)IoRequest->ProcessId,
			(const void*)IoRequest->AddressFrom,
			IoRequest->Buffer,
			IoRequest->ReadSize
		);

		break;
	}
	case IO_CTL_WRITEMEMORY:
	{
		PIO_REQUEST_WRITE_MEMORY IoRequest = (PIO_REQUEST_WRITE_MEMORY)SystemBuffer;
		IoInfo = sizeof(*IoRequest);

		NTSTATUS WriteStatus = KernelWriteVirtualMemory(
			(HANDLE)IoRequest->ProcessId,
			(void*)IoRequest->AddressTo,
			IoRequest->Buffer,
			IoRequest->WriteSize
		);

		break;
	}
	default:
	{
		status = STATUS_FAIL_CHECK;
		break;
	}
	}

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = IoInfo;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}
