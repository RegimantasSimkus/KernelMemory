#include "events.h"
#include "processdata.h"
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

	DbgPrintEx(0, 0, "Handling Device I/O...");

	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	ULONG IoControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
	PVOID SystemBuffer = Irp->AssociatedIrp.SystemBuffer;

	NTSTATUS status = STATUS_SUCCESS;
	ULONG IoInfo = 0;
	switch (IoControlCode)
	{
	case IO_CTL_GETPROCESSHANDLE:
	{
		PIO_REQUEST_PROCESS_HANDLE IoRequest = (PIO_REQUEST_PROCESS_HANDLE)SystemBuffer;
		IoInfo = sizeof(*IoRequest);

		ExAcquireFastMutex(&CS2DataMutex);
		DbgPrintEx(0, 0, "IO_CTL ProcHandle: ProcessId(%.8X); ClientDll(%.8X)", CS2ProcessId, CS2ClientDLL);

		IoRequest->ProcessId = CS2ProcessId;
		IoRequest->ClientDLL = CS2ClientDLL;
		ExReleaseFastMutex(&CS2DataMutex);
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

		DbgPrintEx(0, 0, "I/O VM Read Status - %X", ReadStatus);
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

		DbgPrintEx(0, 0, "I/O VM Write Status - %X", WriteStatus);
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
