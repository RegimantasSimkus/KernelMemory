#pragma once
#include <ntifs.h>
#include <ntddk.h>

VOID ProcessNotifyRoutine(
    PUNICODE_STRING FullImageName,
    HANDLE ProcessId,
    PIMAGE_INFO ImageInfo
);