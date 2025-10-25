#include "routines.h"
#include "process.h"

static const WCHAR* GAME_FOLDERS[2] = {
    L"Counter-Strike Global Offensive\\game\\csgo\\bin\\win64\\",
    L"Counter-Strike Global Offensive\\game\\bin\\win64\\"
};

VOID ProcessNotifyRoutine(
    PUNICODE_STRING FullImageName,
    HANDLE ProcessId,
    PIMAGE_INFO ImageInfo
)
{
    if (!FullImageName)
        return;
    
    for (int i = 0; i < ARRAYSIZE(GAME_FOLDERS); i++)
    {
        const WCHAR* GameFolder = GAME_FOLDERS[i];

        PWCHAR GamePath = wcsstr(FullImageName->Buffer, GameFolder);
        if (!GamePath)
            continue;

        PWCHAR FileName = GamePath + wcslen(GameFolder);

        // longer than .dll
        if (wcslen(FileName) > 4)
        {
            DbgPrintEx(0, 0, "Counter-Strike ClientDLL Loaded: %ls (Proc %X; Base %X)", FileName, ProcessId, ImageInfo->ImageBase);

            g_ProcessId = (ULONG_PTR)ProcessId;
            InsertModuleToList(FileName, (ULONG_PTR)ImageInfo->ImageBase);

            //ExAcquireFastMutex(&CS2DataMutex);
            //CS2ProcessId = (ULONG_PTR)ProcessId;
            //CS2ClientDLL = (ULONG_PTR)ImageInfo->ImageBase;
            //ExReleaseFastMutex(&CS2DataMutex);
        }
    }
}

// Counter - Strike ClientDLL Loaded : \Device\HarddiskVolume3\Program Files(x86)\Steam\steamapps\common\Counter - Strike Global Offensive\game\csgo\bin\win64\server.dll(Proc 1570; Base 3550000)
// Counter-Strike ClientDLL Loaded: \Device\HarddiskVolume3\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\bin\win64\client.dll (Proc 1570; Base 1520000)
