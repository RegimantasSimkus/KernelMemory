#include "CSPawn.h"
#include "../offsets.h"

using namespace cs2_dumper::schemas::client_dll;

int CSPawn::GetHealth()
{
	return ReadType<int>(C_BaseEntity::m_iHealth);
}

Vector3 CSPawn::GetOrigin()
{
	return ReadType<Vector3>(C_BasePlayerPawn::m_vOldOrigin);
}

int CSPawn::GetTeam()
{
	return ReadType<int>(C_BaseEntity::m_iTeamNum);
}