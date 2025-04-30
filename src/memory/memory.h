#pragma once
#include "../core/ds.h"
#include <vector>

class Memory {
public:
	Memory();

	// Read from Ram
	byte readMem(word addr);

	// Write to Ram
	void writeMem(word addr, byte data);

	std::vector<byte> rom;
	std::vector<byte> ext_ram;
	std::vector<byte> vram;	// 8 KiB Video RAM
	std::vector<byte> wram;	// 8 KiB Work RAM
	std::vector<byte> hram;; 	// 127 Bytes High RAM

	byte ram[0x10000];

private:
	PPUState dma_block = PPUState::NONE;
};