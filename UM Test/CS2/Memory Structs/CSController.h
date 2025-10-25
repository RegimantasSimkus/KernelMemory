#pragma once
#include "../MemoryObject.h"

class CSPawn;

class CSController : public MemoryObject
{
public:
	using MemoryObject::MemoryObject;

	unsigned int GetPawn();
};