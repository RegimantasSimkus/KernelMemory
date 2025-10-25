#pragma once
#include "CSMemory.h"
#include "Memory Structs/CSPawn.h"

class CS2
{
public:
	static CSPawn GetLocalPlayerPawn();

	static CSPawn GetPawnIndex(int i);
};