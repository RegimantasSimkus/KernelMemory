#include "memory.h"

NTSTATUS KernelReadVirtualMemory(
    HANDLE TargetProcessId,
    CONST VOID* TargetAddress,
    PVOID Buff,
    SIZE_T BufferSize
)
{
    NTSTATUS status;
    PEPROCESS Process;
    SIZE_T BytesCopied = 0;

    status = PsLookupProcessByProcessId(TargetProcessId, &Process);
    if (NT_SUCCESS(status))
    {
        status = MmCopyVirtualMemory(
            Process,
            TargetAddress,
            PsGetCurrentProcess(),
            Buff,
            BufferSize,
            KernelMode,
            &BytesCopied
        );
    }

    return status;
}

NTSTATUS KernelWriteVirtualMemory(
    HANDLE TargetProcessId,
    PVOID TargetAddress,
    PVOID Buff,
    SIZE_T BufferSize
) 
{
    NTSTATUS status;
    PEPROCESS Process;
    SIZE_T BytesCopied = 0;

    status = PsLookupProcessByProcessId(TargetProcessId, &Process);
    if (NT_SUCCESS(status))
    {
        status = MmCopyVirtualMemory(
            PsGetCurrentProcess(),
            Buff,
            Process,
            TargetAddress,
            BufferSize,
            KernelMode,
            &BytesCopied
        );
    }

    return status;
}
