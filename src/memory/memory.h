#pragma once
#include "../core/ds.h"

class Memory {
public:
	Memory();

	// Get from Address
	byte readMem(word addr) { return ram[addr]; }

	// Write to Ram
	void writeMem(word addr, byte data);

	byte ram[0x10000];
};