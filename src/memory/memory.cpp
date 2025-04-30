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

Memory::Memory() :
		rom(0x8000),
		ext_ram(0x2000),
		oam(0xA0),
		vram(0x2000),
		wram(0x2000),
		hram(0x7F) {
	io[P1] = 0xCF;
	io[SB] = 0x00;
	io[SC] = 0x7E;
	io[DIV] = 0xAB;
	io[TIMA] = 0x00;
	io[TMA] = 0x00;
	io[TAC] = 0xF8;
	io[IF] = 0xE1;
	io[NR10] = 0x80;
	io[NR11] = 0xBF;
	io[NR12] = 0xF3;
	io[NR13] = 0xFF;
	io[NR14] = 0xBF;
	io[NR21] = 0x3F;
	io[NR22] = 0x00;
	io[NR23] = 0xFF;
	io[NR24] = 0xBF;
	io[NR30] = 0x7F;
	io[NR31] = 0xFF;
	io[NR32] = 0x9F;
	io[NR33] = 0xff;
	io[NR34] = 0xBF;
	io[NR41] = 0xFF;
	io[NR42] = 0x00;
	io[NR43] = 0x00;
	io[NR44] = 0xBF;
	io[NR50] = 0x77;
	io[NR51] = 0xF3;
	io[NR52] = 0xF1;
	io[LCDC] = 0x91;
	io[STAT] = 0x85;
	io[SCY] = 0x00;
	io[SCX] = 0x00;
	io[LY] = 0x00;
	io[LYC] = 0x00;
	io[DMA] = 0xFF;
	io[BGP] = 0xFC;
	io[OBP0] = 0x00;
	io[OBP1] = 0x00;
	io[WY] = 0x00;
	io[WX] = 0x00;
	for (word i = 0xff4d; i < 0xffff; i++) {
		ram[i] = 0;
	}
}

// Read from Ram
byte Memory::readMem(word addr) {
	if (addr < 0x8000) {
		// TODO: ROM Area
		return rom[addr];
	}
	else if (addr < 0xA000) {
		// Video Ram
		return vram[addr - 0x8000];
	}
	else if (addr < 0xFF00) {
		if (addr < 0xC000) {
			// External Ram
			return ext_ram[addr-0xA000];
		}
		else if (addr < 0xE000) {
			// Work Ram
			return wram[addr - 0xC000];
		}
		else if (addr < 0xFE00) {
			// Echo of Work Ram
			return wram[addr - 0xE000];
		}
		else if (addr < 0xFEA0) {
			// OAM
			return ram[addr];
		}
		else {
			return 0xFF;
		}
	}
	else if (addr < 0xFF80) {
		// IO Registers
		return ram[addr];
	}
	else if (addr < 0xFFFF) {
		// High Ram
		return hram[addr - 0xFF80];
	}
	else {
		return ram[addr];
	}
}

// Write to Ram
void Memory::writeMem(word addr, byte data) {
	if (addr < 0x8000) {
		// Inaccessible ROM
		rom[addr] = data;
	}
	else if (addr < 0xA000) {
		// Video Ram
		vram[addr - 0x8000] = data;
	}
	else if (addr < 0xFF00) {
		if (addr < 0xC000) {
			// External Ram
			ext_ram[addr] = data;
		}
		else if (addr < 0xE000) {
			// Work Ram
			wram[addr - 0xC000] = data;
		}
		else if (addr < 0xFE00) {
			// Echo of Work Ram
			wram[addr - 0xE000] = data;
		}
		else if (addr < 0xFEA0) {
			// OAM
			oam[addr - 0xFE00] = data;
		}
	}
	else if (addr < 0xFF80) {
		// IO Registers
		// Do something special
		writeIO(addr, data);
	}
	else if (addr < 0xFFFF) {
		// High Ram
		hram[addr - 0xFF80] = data;
	}
	else {
		// Interrupt Enable
		io[IE] = data;
	}
	//ram[addr] = data;
}

byte Memory::readIO(word addr) {
	byte ioAddr = addr & 0xFF;
	switch (ioAddr) {
		case P1: {
			return io[P1] & 0xC0;
		}
		case SB: {
			return io[SB];
		}
		case SC: {
			return io[SC] | 0x7E;
		}
		case DIV: {
			return io[DIV];
		}
		case TIMA: {
			return io[TIMA];
		}
		case TMA: {
			return io[TMA];
		}
		case TAC: {
			return io[TAC];
		}
		case IF: {
			return io[IF];
		}
		case NR10: case NR11: case NR12: {
			return io[ioAddr];
		}
		case NR13: return 0xFF; 
		case NR14: case NR21: case NR22: {
			return io[ioAddr];
		}
		case NR23: return 0xFF;
		case NR24: case NR30: {
			return io[ioAddr];
		}
		case NR31: return 0xFF;
		case NR32: {
			return io[ioAddr];
		}
		case NR33: return 0xFF;
		case NR34: {
			return io[ioAddr];
		}
		case NR41: return 0xFF;
		case NR42: case NR43: case NR44:
		case NR50: case NR51: case NR52: {
			return io[ioAddr];
		}
		case LCDC: {
			return io[LCDC];
		}
		case STAT: {
			return io[STAT] | 0x80;
		}
		case SCY: case SCX:
			return io[ioAddr];

		case LY:
			return 0x00;

		case LYC:
			return io[LYC];
		case DMA:
			return io[DMA];
		case BGP:
			return io[BGP];
		case OBP0: case OBP1:
			return io[ioAddr];
		case WY: case WX:
			return io[ioAddr];
		default: {
			printf("Unknown IO Register: %x\n", ioAddr);
			return 0xFF;
		}
	}
}

void Memory::writeIO(word addr, byte data) {
	byte ioAddr = addr & 0xFF;
	switch (ioAddr) {
		case P1: {
			io[P1] = (io[P1] & 0x0F) | (data & 0x30);
			break;
		}
		case SB: {
			io[SB] = data;
			break;
		}
		case SC: {
			io[SC] = data & 0x81;
			break;
		}
		case DIV: {
			io[DIV] = 0;
			break;
		}
		case TIMA: {
			io[TIMA] = data;
			break;
		}
		case TMA: {
			io[TMA] = data;
			break;
		}
		case TAC: {
			io[TAC] = data & 0x07;
			break;
		}
		case IF: {
			io[IF] = data & 0x1F;
			break;
		}
		case NR10: case NR11: case NR12: case NR13: case NR14:
		case NR21: case NR22: case NR23: case NR24:
		case NR30: case NR31: case NR32: case NR33: case NR34:
		case NR41: case NR42: case NR43: case NR44:
		case NR50: case NR51: case NR52: {
			io[ioAddr] = data;
			break;
		}
		case LCDC: {
			io[LCDC] = data;
			break;
		}
		case STAT: {
			io[STAT] = (data & 0x78) | (io[STAT] & 0x07);
			break;
		}
		case SCY: case SCX: {
			io[ioAddr] = data;
			break;
		}
		case LY: break;
		case LYC: {
			io[LYC] = data;
			break;
		}
		case DMA: {
			io[DMA] = data;
			break;
		}
		case BGP: {
			io[BGP] = data;
			break;
		}
		case OBP0: case OBP1: {
			io[ioAddr] = data & 0xFC;
			break;
		}
		case WY: case WX: {
			io[ioAddr] = data;
			break;
		}
		case IE: {
			io[IE] = data & 0x1F;
			break;
		}
		default: {
			printf("Unknown IO Register: %x\n", ioAddr);
			break;
		}
	}
}