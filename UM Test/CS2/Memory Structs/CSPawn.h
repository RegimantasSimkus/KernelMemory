#pragma once
#include "../MemoryObject.h"
#include "../Engine/Vector3.h"

class CSPawn : public MemoryObject
{
public:
	using MemoryObject::MemoryObject;

	int GetHealth();

	int GetTeam();

	Vector3 GetOrigin();
};