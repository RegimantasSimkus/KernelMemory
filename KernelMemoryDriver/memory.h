#pragma once
#include <ntifs.h>

// ntoskrnl.exe
// https://github.com/x-tinkerer/WRK/blob/e2e25706c766e1f93b3e55ab95601e72860f74d9/base/ntos/mm/readwrt.c#L360
NTSTATUS MmCopyVirtualMemory(
    IN PEPROCESS FromProcess,
    IN CONST VOID* FromAddress,
    IN PEPROCESS ToProcess,
    OUT PVOID ToAddress,
    IN SIZE_T BufferSize,
    IN KPROCESSOR_MODE PreviousMode,
    OUT PSIZE_T NumberOfBytesCopied
);

NTSTATUS KernelReadVirtualMemory(
    HANDLE FromProcessId,
    CONST VOID* FromAddress,
    PVOID ToAddress,
    SIZE_T BufferSize
);

NTSTATUS KernelWriteVirtualMemory(
    HANDLE ToProcessId,
    CONST VOID* ToAddress,
    PVOID FromAddress,
    SIZE_T BufferSize
);
