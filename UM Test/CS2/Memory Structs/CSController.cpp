#include "CSController.h"
#include "CSPawn.h"

#include "../offsets.h"

using namespace cs2_dumper::schemas::client_dll;

unsigned int CSController::GetPawn()
{
	return ReadType<unsigned int>(CCSPlayerController::m_hPlayerPawn);
}