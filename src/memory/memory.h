#pragma once
#include "../core/ds.h"
#include <vector>

class Memory {
public:
	Memory();
	Memory(std::vector<byte> _rom);

	// Read from Ram
	byte readMem(word addr);

	// Write to Ram
	void writeMem(word addr, byte data);

	byte readIO(word addr);

	// Write to IO
	void writeIO(word addr, byte data);

	std::vector<byte> rom;		// Cartridge ROM
	std::vector<byte> ext_ram;	// External  Cartridge RAM
	std::vector<byte> oam;		// Object Attribute Memory
	std::vector<byte> vram;		// 8 KiB Video RAM
	std::vector<byte> wram;		// 8 KiB Work RAM
	std::vector<byte> hram;		// 127 Bytes High RAM

	byte ram[0x10000];
	byte io[0x100]{ 0 };

private:
	PPUState dma_block = PPUState::NONE;
};