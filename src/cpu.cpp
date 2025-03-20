#include "cpu.h"

CPU::CPU(Memory& _ram) : ram(_ram) {
	registers.reg8[A] = 0x01; registers.reg8[F] = FLAG_Z;
	registers.reg16[BC] = 0x0013;
	registers.reg16[DE] = 0x00D8;
	registers.reg16[HL] = 0x014D;
	registers.reg16[SP] = 0xFFFE;
}

// Reigster Functions
// ------------------

// Get 8 Bit Register
byte CPU::get8(reg8 reg) { return registers.reg8[reg]; }

// Get 16 Bit Register
word CPU::get16(reg16 reg) {return registers.reg16[reg]; }


// Set 8 Bit Register
void CPU::set8(reg8 reg, byte b) { registers.reg8[reg] = b; }

// Set 16 Bit Register
void CPU::set16(reg16 reg, word w) { registers.reg16[reg] = w; }

// Set 16 Bit Register
void CPU::set16(reg16 reg, byte l, byte h) { registers.reg16[reg] = (l | (h << 8)); }


// Get Z Flag
bool CPU::getZ() { return registers.reg8[F] >> 7; }

// Get N Flag
bool CPU::getN() { return (registers.reg8[F] & 0b01000000) >> 6; }

// Get H Flag
bool CPU::getH() { return (registers.reg8[F] & 0b00100000) >> 5; }

// Get C Flag
bool CPU::getC() { return (registers.reg8[F] & 0b00010000) >> 4; }


// Set Z Flag
void CPU::setZ(bool z) { if (z) { set8(F, flagZ); } else { registers.reg8[F] &= ~flagZ; } }

// Set N Flag
void CPU::setN(bool n) { if (n) { set8(F, flagN); } else { registers.reg8[F] &= ~flagN; } }

// Set H Flag
void CPU::setH(bool h) { if (h) { set8(F, flagH); } else { registers.reg8[F] &= ~flagH; } }

// Set C Flag
void CPU::setC(bool c) { if (c) { set8(F, flagC); } else { registers.reg8[F] &= ~flagC; } }
// -------------------

// Ram Functions
// -------------
// TODO make safer functions!

// Get Ram from Address
byte CPU::getRam(word addr) {
	return ram.ram[addr];
}

// Set Ram at Address
void CPU::setRam(word addr, byte data) {
	ram.ram[addr] = data;
}

// Push Word to Stack
void CPU::push(word w) {
	setRam(--registers.reg16[SP], w & (0xFF00 >> 8));
	setRam(--registers.reg16[SP], w & 0x00FF);
}

// Pop Word from Stack
word CPU::pop() {
	word w = getRam(registers.reg16[SP]++);
	w |= getRam(registers.reg16[SP]++) << 8;
	return w;
}

// -------------

void CPU::exec(byte op) {

	switch (op) {
		// Control Instructions
		case 0x00: { NOP(); return; } case 0x10: { STOP(); return; }
		case 0xCB: { PREF(op); return; } case 0xF3: { DI(); return; }
		case 0xFB: { EI(); return; }

		// Jumps / Calls
		case 0x18: case 0x20: case 0x28: case 0x30: case 0x38: { JR(op); return; }
		case 0xC0: case 0xC8: case 0xC9: case 0xD0: case 0xD8: { RET(); return; }
		case 0xC2: case 0xC3: case 0xCA: case 0xD2: case 0xDA: { JP(op); return; }
	}
}


// Prefix for CB instructions
byte CPU::PREF(byte ins) {
	if (ins == 0) printf("PREF");
	return 4;
}