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
	ram[P1] = 0xCF;
	ram[SB] = 0x00;
	ram[SC] = 0x7E;
	ram[DIV] = 0xAB;
	ram[TIMA] = 0x00;
	ram[TMA] = 0x00;
	ram[TAC] = 0xF8;
	ram[IF] = 0xE1;
	ram[NR10] = 0x80;
	ram[NR11] = 0xBF;
	ram[NR12] = 0xF3;
	ram[NR13] = 0xFF;
	ram[NR14] = 0xBF;
	ram[NR21] = 0x3F;
	ram[NR22] = 0x00;
	ram[NR23] = 0xFF;
	ram[NR24] = 0xBF;
	ram[NR30] = 0x7F;
	ram[NR31] = 0xFF;
	ram[NR32] = 0x9F;
	ram[NR33] = 0xff;
	ram[NR34] = 0xBF;
	ram[NR41] = 0xFF;
	ram[NR42] = 0x00;
	ram[NR43] = 0x00;
	ram[NR44] = 0xBF;
	ram[NR50] = 0x77;
	ram[NR51] = 0xF3;
	ram[NR52] = 0xF1;
	ram[LCDC] = 0x91;
	ram[STAT] = 0x85;
	ram[SCY] = 0x00;
	ram[SCX] = 0x00;
	ram[LY] = 0x00;
	ram[LYC] = 0x00;
	ram[DMA] = 0xFF;
	ram[BGP] = 0xFC;
	ram[OBP0] = 0x00;
	ram[OBP1] = 0x00;
	ram[WY] = 0x00;
	ram[WX] = 0x00;
	for (word i = 0xff4d; i < 0xffff; i++) {
		ram[i] = 0;
	}
}