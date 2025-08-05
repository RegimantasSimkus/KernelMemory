#include "routines.h"
#include "processdata.h"

VOID ProcessNotifyRoutine(
    PUNICODE_STRING FullImageName,
    HANDLE ProcessId,
    PIMAGE_INFO ImageInfo
)
{
    if (!FullImageName)
        return;
    
    if (wcsstr(FullImageName->Buffer, L"Counter-Strike Global Offensive\\game\\csgo\\bin\\win64\\client.dll"))
    {
        DbgPrintEx(0, 0, "Counter-Strike ClientDLL Loaded: %wZ (Proc %X; Base %X)", FullImageName, ProcessId, ImageInfo->ImageBase);

        ExAcquireFastMutex(&CS2DataMutex);
        CS2ProcessId = (ULONG_PTR)ProcessId;
        CS2ClientDLL = (ULONG_PTR)ImageInfo->ImageBase;
        ExReleaseFastMutex(&CS2DataMutex);
    }
}

// Counter - Strike ClientDLL Loaded : \Device\HarddiskVolume3\Program Files(x86)\Steam\steamapps\common\Counter - Strike Global Offensive\game\csgo\bin\win64\server.dll(Proc 1570; Base 3550000)
// Counter-Strike ClientDLL Loaded: \Device\HarddiskVolume3\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\bin\win64\client.dll (Proc 1570; Base 1520000)
