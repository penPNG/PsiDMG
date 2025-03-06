#include "memory.h"

// https://gbdev.io/pandocs/Memory_Map.html
// 0000	3FFF	16 KiB ROM bank 00				From cartridge, usually a fixed bank
// 4000	7FFF	16 KiB ROM Bank 01–NN			From cartridge, switchable bank via mapper(if any)
// 8000	9FFF	8 KiB Video RAM(VRAM)			In CGB mode, switchable bank 0 / 1
// A000	BFFF	8 KiB External RAM				From cartridge, switchable bank if any
// C000	CFFF	4 KiB Work RAM(WRAM)
// D000	DFFF	4 KiB Work RAM(WRAM)			In CGB mode, switchable bank 1–7
// E000	FDFF	Echo RAM(mirror of C000–DDFF)	Nintendo says use of this area is prohibited.
// FE00	FE9F	Object attribute memory(OAM)
// FEA0	FEFF	Not Usable						Nintendo says use of this area is prohibited.
// FF00	FF7F	I / O Registers
// FF80	FFFE	High RAM(HRAM)
// FFFF	FFFF	Interrupt Enable register (IE)

Memory::Memory() {
	ram[0xff00] = 0xCF;
	ram[0xff01] = 0x00;
	ram[0xff02] = 0x7E;
	ram[0xff04] = 0xAB;
	ram[0xff05] = 0x00;
	ram[0xff06] = 0x00;
	ram[0xff07] = 0xF8;
	ram[0xff0f] = 0xE1;
	ram[0xff10] = 0x80;
	ram[0xff11] = 0xBF;
	ram[0xff12] = 0xF3;
	ram[0xff13] = 0xFF;
	ram[0xff14] = 0xBF;
	ram[0xff16] = 0x3F;
	ram[0xff17] = 0x00;
	ram[0xff18] = 0xFF;
	ram[0xff19] = 0xBF;
	ram[0xff1a] = 0x7F;
	ram[0xff1b] = 0xFF;
	ram[0xff1c] = 0x9F;
	ram[0xff1d] = 0xff;
	ram[0xff1e] = 0xBF;
	ram[0xff20] = 0xFF;
	ram[0xff21] = 0x00;
	ram[0xff22] = 0x00;
	ram[0xff23] = 0xBF;
	ram[0xff24] = 0x77;
	ram[0xff25] = 0xF3;
	ram[0xff26] = 0xF1;
	ram[0xff40] = 0x91;
	ram[0xff41] = 0x85;
	ram[0xff42] = 0x00;
	ram[0xff43] = 0x00;
	ram[0xff44] = 0x00;
	ram[0xff45] = 0x00;
	ram[0xff46] = 0xFF;
	ram[0xff47] = 0xFC;
	ram[0xff48] = 0x00;
	ram[0xff49] = 0x00;
	ram[0xff4a] = 0x00;
	ram[0xff4b] = 0x00;
	for (word i = 0xff4d; i < 0xffff; i++) {
		ram[i] = 0;
	}
}