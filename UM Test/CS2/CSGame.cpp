#include "CSGame.h"
#include "offsets.h"

CSPawn CS2::GetLocalPlayerPawn()
{
	return Memory.ReadMemory<ULONG_PTR>(Memory.ClientDll + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
}

CSPawn CS2::GetPawnIndex(int i)
{
	ULONG_PTR entList = Memory.ReadMemory<uintptr_t>(Memory.ClientDll + cs2_dumper::offsets::client_dll::dwEntityList);

	ULONG_PTR entry = Memory.ReadMemory<ULONG_PTR>(entList + (8 * ((i & 0x7FFF) >> 9)) + 0x10);
	if (!entry)
		return 0;

	ULONG_PTR controller = Memory.ReadMemory<ULONG_PTR>(entry + (112 * (i & 0x1FF)));
	if (!controller) 
		return 0;

	unsigned short pawnIndex = Memory.ReadMemory<unsigned short>(controller + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);

	entry = Memory.ReadMemory<ULONG_PTR>(entList + (8 * ((pawnIndex & 0x7FFF) >> 9)) + 0x10);
	if (!entry)
		return 0;

	ULONG_PTR pawn = Memory.ReadMemory<ULONG_PTR>(entry + (112 * (pawnIndex & 0x1FF)));
    return pawn;
}