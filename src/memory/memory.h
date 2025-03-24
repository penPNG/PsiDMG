#pragma once
#include "../core/ds.h"

class Memory {
public:
	Memory();

	byte ram[0x10000];
};