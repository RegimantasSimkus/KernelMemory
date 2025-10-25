#pragma once
#include "CSMemory.h"

class MemoryObject
{
public:
	MemoryObject(unsigned long long base) : Base(base)
	{
	}

	template <typename T>
	T ReadType(int offset = 0)
	{
		T val;
		Memory.ReadMemory<T>(Base + offset, &val);
		return val;
	}

	operator bool()
	{
		return Base != 0;
	}

	bool operator==(const MemoryObject& object)
	{
		return Base == object.Base;
	}

	unsigned long long Base;
};