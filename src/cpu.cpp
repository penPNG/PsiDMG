#include "cpu.h"

CPU::CPU(Memory& _ram) : ram(_ram) {
	registers.reg8[A] = 0x01; registers.reg8[F] = flagZ|flagH|flagC;
	registers.reg16[BC] = 0x0013;
	registers.reg16[DE] = 0x00D8;
	registers.reg16[HL] = 0x014D;
	registers.reg16[SP] = 0xFFFE;
	PC = 0x0100;
	IME = 1;
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

byte CPU::exec(byte op) {

	switch (op) {
		// Control Instructions
		case 0x00: { return NOP(); } case 0x10: { return STOP(); }
		case 0xCB: { return PREF(op); } case 0xF3: { return  DI(); }
		case 0xFB: { return EI(); } case 0x76: { return HALT(); }

		// Load Instructions
				 // Register Memory Loads
		case 0x02: { return LDMR(get16(BC), A); } case 0x12: { return LDMR(get16(DE), A); } case 0x22: { return LDMR(registers.reg16[HL]++, A); } case 0x32: { return LDMR(registers.reg16[HL]--, A); }
		case 0x0A: { return LDRM(A, get16(BC)); } case 0x1A: { return LDRM(A, get16(DE)); } case 0x2A: { return LDRM(A, registers.reg16[HL]++); } case 0x3A: { return LDRM(A, registers.reg16[HL]--); }
				 // Immediate Loads
		case 0x06: { return LDI(B, op); } case 0x16: { return LDI(D, op); } case 0x26: { return LDI(H, op); } case 0x36: { return LDMI(get16(HL), op); }
		case 0x0E: { return LDI(C, op); } case 0x1E: { return LDI(E, op); } case 0x2E: { return LDI(L, op); } case 0x3E: { return LDI(A, op); }
				 // 0x4X
		case 0x40: { return LD(B, B); } case 0x41: { return LD(B, C); } case 0x42: { return LD(B, D); } case 0x43: { return LD(B, E); }
		case 0x44: { return LD(B, H); } case 0x45: { return LD(B, L); } case 0x46: { return LDRM(B, get16(HL)); } case 0x47: { return LD(B, A); }
		case 0x48: { return LD(C, B); } case 0x49: { return LD(C, C); } case 0x4A: { return LD(C, D); } case 0x4B: { return LD(C, E); }
		case 0x4C: { return LD(C, H); } case 0x4D: { return LD(C, L); } case 0x4E: { return LDRM(C, get16(HL)); } case 0x4F: { return LD(C, A); }
				 // 0x5X
		case 0x50: { return LD(D, B); } case 0x51: { return LD(D, C); } case 0x52: { return LD(D, D); } case 0x53: { return LD(D, E); }
		case 0x54: { return LD(D, H); } case 0x55: { return LD(D, L); } case 0x56: { return LDRM(D, get16(HL)); } case 0x57: { return LD(D, A); }
		case 0x58: { return LD(E, B); } case 0x59: { return LD(E, C); } case 0x5A: { return LD(E, D); } case 0x5B: { return LD(E, E); }
		case 0x5C: { return LD(E, H); } case 0x5D: { return LD(E, L); } case 0x5E: { return LDRM(E, get16(HL)); } case 0x5F: { return LD(E, A); }
				 // 0x6X
		case 0x60: { return LD(H, B); } case 0x61: { return LD(H, C); } case 0x62: { return LD(H, D); } case 0x63: { return LD(H, E); }
		case 0x64: { return LD(H, H); } case 0x65: { return LD(H, L); } case 0x66: { return LDRM(H, get16(HL)); } case 0x67: { return LD(H, A); }
		case 0x68: { return LD(L, B); } case 0x69: { return LD(L, C); } case 0x6A: { return LD(L, D); } case 0x6B: { return LD(L, E); }
		case 0x6C: { return LD(L, H); } case 0x6D: { return LD(L, L); } case 0x6E: { return LDRM(L, get16(HL)); } case 0x6F: { return LD(L, A); }
				 // 0x7X
		case 0x70: { return LDMR(get16(HL), B); } case 0x71: { return LDMR(get16(HL), C); } case 0x72: { return LDMR(get16(HL), D); } case 0x73: { return LDMR(get16(HL), E); }
		case 0x74: { return LDMR(get16(HL), H); } case 0x75: { return LDMR(get16(HL), L); } /*case 0x76: { HALT }*/ case 0x77: { return LDMR(get16(HL), A); }
		case 0x78: { return LD(A, B); } case 0x79: { return LD(A, C); } case 0x7A: { return LD(A, D); } case 0x7B: { return LD(A, E); }
		case 0x7C: { return LD(A, H); } case 0x7D: { return LD(A, L); } case 0x7E: { return LDRM(A, get16(HL)); } case 0x7F: { return LD(A, A); }
				 // Relative Memory Loads
		case 0xE0: { return LDAM(op); } case 0xF0: { return LDMA(op); }
		case 0xE2: { return LDMR(0xFF00 + get8(C), A); } case 0xF2: { return LDRM(A, 0xFF00 + get8(C)); }
				 // Immediate Memory Loads
		case 0xEA: { return LDIA(op); } case 0xFA: { return LDAI(op); }
				 // 16 Bit Loads
		case 0x01: { return LD16(BC, op); } case 0x11: { return LD16(DE, op); } case 0x21: { return LD16(HL, op); } case 0x31: { return LD16(SP, op); }
		case 0x08: { return LD16MSP(op); }
				 // Special Loads
		case 0xF8: { return LD16HLSP(op); } case 0xF9: { return LD16SPHL(); }
				 // Push / Pop
		case 0xC1: { return POP(BC); } case 0xD1: { return POP(DE); } case 0xE1: { return POP(HL); } case 0xF1: { return POP(AF); }
		case 0xC5: { return PUSH(BC); } case 0xD5: { return PUSH(DE); } case 0xE5: { return PUSH(HL); } case 0xF5: { return PUSH(AF); }

		// Arithmetic Instructions
				 // 0x8X
		case 0x80: { return ADD(B); } case 0x81: { return ADD(C); } case 0x82: { return ADD(D); } case 0x83: { return ADD(E); }
		case 0x84: { return ADD(H); } case 0x85: { return ADD(L); } case 0x86: { return ADDM(get16(HL)); } case 0x87: { return ADD(A); }
		case 0x88: { return ADC(B); } case 0x89: { return ADC(C); } case 0x8A: { return ADC(D); } case 0x8B: { return ADC(E); }
		case 0x8C: { return ADC(H); } case 0x8D: { return ADC(L); } case 0x8E: { return ADCM(get16(HL)); } case 0x8F: { return ADC(A); }
				 // 0x9X
		case 0x90: { return SUB(B); } case 0x91: { return SUB(C); } case 0x92: { return SUB(D); } case 0x93: { return SUB(E); }
		case 0x94: { return SUB(H); } case 0x95: { return SUB(L); } case 0x96: { return SUBM(get16(HL)); } case 0x97: { return SUB(A); }
		case 0x98: { return SBC(B); } case 0x99: { return SBC(C); } case 0x9A: { return SBC(D); } case 0x9B: { return SBC(E); }
		case 0x9C: { return SBC(H); } case 0x9D: { return SBC(L); } case 0x9E: { return SBCM(get16(HL)); } case 0x9F: { return SBC(A); }
				 // 0xAX
		case 0xA0: { return AND(B); } case 0xA1: { return AND(C); } case 0xA2: { return AND(D); } case 0xA3: { return AND(E); }
		case 0xA4: { return AND(H); } case 0xA5: { return AND(L); } case 0xA6: { return ANDM(get16(HL)); } case 0xA7: { return AND(A); }
		case 0xA8: { return XOR(B); } case 0xA9: { return XOR(C); } case 0xAA: { return XOR(D); } case 0xAB: { return XOR(E); }
		case 0xAC: { return XOR(H); } case 0xAD: { return XOR(L); } case 0xAE: { return XORM(get16(HL)); } case 0xAF: { return XOR(A); }
				 // 0xBX
		case 0xB0: { return OR(B); } case 0xB1: { return OR(C); } case 0xB2: { return OR(D); } case 0xB3: { return OR(E); }
		case 0xB4: { return OR(H); } case 0xB5: { return OR(L); } case 0xB6: { return ORM(get16(HL)); } case 0xB7: { return OR(A); }
		case 0xB8: { return CP(B); } case 0xB9: { return CP(C); } case 0xBA: { return CP(D); } case 0xBB: { return CP(E); }
		case 0xBC: { return CP(H); } case 0xBD: { return CP(L); } case 0xBE: { return CPM(get16(HL)); } case 0xBF: { return CP(A); }
				 // Increment
		case 0x04: { return INC(B); } case 0x0C: { return INC(C); } case 0x14: { return INC(D); } case 0x1C: { return INC(E); }
		case 0x24: { return INC(H); } case 0x2C: { return INC(L); } case 0x34: { return INCM(get16(HL)); } case 0x3C: { return INC(A); }
				 // Decrement
		case 0x05: { return DEC(B); } case 0x0D: { return DEC(C); } case 0x15: { return DEC(D); } case 0x1D: { return DEC(E); }
		case 0x25: { return DEC(H); } case 0x2D: { return DEC(L); } case 0x35: { return DECM(get16(HL)); } case 0x3D: { return DEC(A); }
				 // Immediates
		case 0xC6: { return ADDI(op); } case 0xCE: { return ADCI(op); } case 0xD6: { return SUBI(op); } case 0xDE: { return SBCI(op); }
		case 0xE6: { return ANDI(op); } case 0xEE: { return XORI(op); } case 0xF6: { return ORI(op); } case 0xFE: { return CPI(op); }
				 // Special Arithmetic
		case 0x27: { return DAA(); } case 0x2F: { return CPL(); } case 0x37: { return SCF(); } case 0x3F: { return CCF(); }
				 // 16 Bit Increment
		case 0x03: { return INC16(BC); } case 0x13: { return INC16(DE); } case 0x23: { return INC16(HL); } case 0x33: { return INC16(SP); }
				 // 16 Bit Decrement
		case 0x0B: { return DEC16(BC); } case 0x1B: { return DEC16(DE); } case 0x2B: { return DEC16(HL); } case 0x3B: { return DEC16(SP); }
				 // 16 Bit Add
		case 0x09: { return ADD16(BC); } case 0x19: { return ADD16(DE); } case 0x29: { return ADD16(HL); } case 0x39: { return ADD16(SP); }
				 // 16 Bit Add Signed Byte
		case 0xE8: { return ADD16I(op); }

		// Jumps / Calls
				 // Relative Jumps
		case 0x18: { return JR(op); }
		case 0x20: { return JRN(flagZ, op); } case 0x28: { return JRS(flagZ, op); } case 0x30: { return JRN(flagC, op); } case 0x38: { return JRS(flagC, op); }
				 // Absolute Jumps
		case 0xC3: { return JP(op); } case 0xE9: { return JPHL(); }
		case 0xC2: { return JPN(flagZ, op); } case 0xCA: { return JPS(flagZ, op); } case 0xD2: { return JPN(flagC, op); } case 0xDA: { return JPS(flagC, op); }
				 // Calls
		case 0xCD: { return CALL(op); }
		case 0xC4: { return CLLN(flagZ, op); } case 0xCC: { return CLLS(flagZ, op); } case 0xD4: { return CLLN(flagC, op); } case 0xDC: { return CLLS(flagC, op); }
				 // Returns
		case 0xC9: { return RET(); } case 0xD9: { return RETI(); }
		case 0xC0: { return RTN(flagZ); } case 0xC8: { return RTS(flagZ); } case 0xD0: { return RTN(flagC); } case 0xD8: { return RTS(flagC); }
				 // Resets
		case 0xC7: { return RST(0x00); } case 0xCF: { return RST(0x08); } case 0xD7: { return RST(0x10); } case 0xDF: { return RST(0x18); }
		case 0xE7: { return RST(0x20); } case 0xEF: { return RST(0x28); } case 0xF7: { return RST(0x30); } case 0xFF: { return RST(0x38); }
	}
	return 4;
}


// Prefix for CB instructions
byte CPU::PREF(byte ins) {
	if (ins == 0) printf("PREF");
	//printf("%x", PC);
	byte c;
	switch (ram.ram[PC++]) {
		// Rotates
		case 0x00: { return RLC(B); } case 0x01: { return RLC(C); } case 0x02: { return RLC(D); } case 0x03: { return RLC(E); }
		case 0x04: { return RLC(H); } case 0x05: { return RLC(L); } case 0x06: { return RLCM(get16(HL)); } case 0x07: { return RLC(A); }
		case 0x08: { return RRC(B); } case 0x09: { return RRC(C); } case 0x0A: { return RRC(D); } case 0x0B: { return RRC(E); }
		case 0x0C: { return RRC(H); } case 0x0D: { return RRC(L); } case 0x0E: { return RRCM(get16(HL)); } case 0x0F: { return RRC(A); }
		case 0x10: { return RL(B); } case 0x11: { return RL(C); } case 0x12: { return RL(D); } case 0x13: { return RL(E); }
		case 0x14: { return RL(H); } case 0x15: { return RL(L); } case 0x16: { return RLM(get16(HL)); } case 0x17: { return RL(A); }
		case 0x18: { return RR(B); } case 0x19: { return RR(C); } case 0x1A: { return RR(D); } case 0x1B: { return RR(E); }
		case 0x1C: { return RR(H); } case 0x1D: { return RR(L); } case 0x1E: { return RRM(get16(HL)); } case 0x1F: { return RR(A); }
				 // Shifts / Swaps
		case 0x20: { return SLA(B); } case 0x21: { return SLA(C); } case 0x22: { return SLA(D); } case 0x23: { return SLA(E); }
		case 0x24: { return SLA(H); } case 0x25: { return SLA(L); } case 0x26: { return SLAM(get16(HL)); } case 0x27: { return SLA(A); }
		case 0x28: { return SRA(B); } case 0x29: { return SRA(C); } case 0x2A: { return SRA(D); } case 0x2B: { return SRA(E); }
		case 0x2C: { return SRA(H); } case 0x2D: { return SRA(L); } case 0x2E: { return SRAM(get16(HL)); } case 0x2F: { return SRA(A); }
		case 0x30: { return SWAP(B); } case 0x31: { return SWAP(C); } case 0x32: { return SWAP(D); } case 0x33: { return SWAP(E); }
		case 0x34: { return SWAP(H); } case 0x35: { return SWAP(L); } case 0x36: { return SWPM(get16(HL)); } case 0x37: { return SWAP(A); }
		case 0x38: { return SRL(B); } case 0x39: { return SRL(C); } case 0x3A: { return SRL(D); } case 0x3B: { return SRL(E); }
		case 0x3C: { return SRL(H); } case 0x3D: { return SRL(L); } case 0x3E: { return SRLM(get16(HL)); } case 0x3F: { return SRL(A); }
				 // Bit Tests
		case 0x40: { return BIT(0, B); } case 0x41: { return BIT(0, C); } case 0x42: { return BIT(0, D); } case 0x43: { return BIT(0, E); }
		case 0x44: { return BIT(0, H); } case 0x45: { return BIT(0, L); } case 0x46: { return BITM(0, get16(HL)); } case 0x47: { return BIT(0, A); }
		case 0x48: { return BIT(1, B); } case 0x49: { return BIT(1, C); } case 0x4A: { return BIT(1, D); } case 0x4B: { return BIT(1, E); }
		case 0x4C: { return BIT(1, H); } case 0x4D: { return BIT(1, L); } case 0x4E: { return BITM(1, get16(HL)); } case 0x4F: { return BIT(1, A); }
		case 0x50: { return BIT(2, B); } case 0x51: { return BIT(2, C); } case 0x52: { return BIT(2, D); } case 0x53: { return BIT(2, E); }
		case 0x54: { return BIT(2, H); } case 0x55: { return BIT(2, L); } case 0x56: { return BITM(2, get16(HL)); } case 0x57: { return BIT(2, A); }
		case 0x58: { return BIT(3, B); } case 0x59: { return BIT(3, C); } case 0x5A: { return BIT(3, D); } case 0x5B: { return BIT(3, E); }
		case 0x5C: { return BIT(3, H); } case 0x5D: { return BIT(3, L); } case 0x5E: { return BITM(3, get16(HL)); } case 0x5F: { return BIT(3, A); }
		case 0x60: { return BIT(4, B); } case 0x61: { return BIT(4, C); } case 0x62: { return BIT(4, D); } case 0x63: { return BIT(4, E); }
		case 0x64: { return BIT(4, H); } case 0x65: { return BIT(4, L); } case 0x66: { return BITM(4, get16(HL)); } case 0x67: { return BIT(4, A); }
		case 0x68: { return BIT(5, B); } case 0x69: { return BIT(5, C); } case 0x6A: { return BIT(5, D); } case 0x6B: { return BIT(5, E); }
		case 0x6C: { return BIT(5, H); } case 0x6D: { return BIT(5, L); } case 0x6E: { return BITM(5, get16(HL)); } case 0x6F: { return BIT(5, A); }
		case 0x70: { return BIT(6, B); } case 0x71: { return BIT(6, C); } case 0x72: { return BIT(6, D); } case 0x73: { return BIT(6, E); }
		case 0x74: { return BIT(6, H); } case 0x75: { return BIT(6, L); } case 0x76: { return BITM(6, get16(HL)); } case 0x77: { return BIT(6, A); }
		case 0x78: { return BIT(7, B); } case 0x79: { return BIT(7, C); } case 0x7A: { return BIT(7, D); } case 0x7B: { return BIT(7, E); }
		case 0x7C: { return BIT(7, H); } case 0x7D: { return BIT(7, L); } case 0x7E: { return BITM(7, get16(HL)); } case 0x7F: { return BIT(7, A); }
				 // Bit Resets
		case 0x80: { return RES(0, B); } case 0x81: { return RES(0, C); } case 0x82: { return RES(0, D); } case 0x83: { return RES(0, E); }
		case 0x84: { return RES(0, H); } case 0x85: { return RES(0, L); } case 0x86: { return RESM(0, get16(HL)); } case 0x87: { return RES(0, A); }
		case 0x88: { return RES(1, B); } case 0x89: { return RES(1, C); } case 0x8A: { return RES(1, D); } case 0x8B: { return RES(1, E); }
		case 0x8C: { return RES(1, H); } case 0x8D: { return RES(1, L); } case 0x8E: { return RESM(1, get16(HL)); } case 0x8F: { return RES(1, A); }
		case 0x90: { return RES(2, B); } case 0x91: { return RES(2, C); } case 0x92: { return RES(2, D); } case 0x93: { return RES(2, E); }
		case 0x94: { return RES(2, H); } case 0x95: { return RES(2, L); } case 0x96: { return RESM(2, get16(HL)); } case 0x97: { return RES(2, A); }
		case 0x98: { return RES(3, B); } case 0x99: { return RES(3, C); } case 0x9A: { return RES(3, D); } case 0x9B: { return RES(3, E); }
		case 0x9C: { return RES(3, H); } case 0x9D: { return RES(3, L); } case 0x9E: { return RESM(3, get16(HL)); } case 0x9F: { return RES(3, A); }
		case 0xA0: { return RES(4, B); } case 0xA1: { return RES(4, C); } case 0xA2: { return RES(4, D); } case 0xA3: { return RES(4, E); }
		case 0xA4: { return RES(4, H); } case 0xA5: { return RES(4, L); } case 0xA6: { return RESM(4, get16(HL)); } case 0xA7: { return RES(4, A); }
		case 0xA8: { return RES(5, B); } case 0xA9: { return RES(5, C); } case 0xAA: { return RES(5, D); } case 0xAB: { return RES(5, E); }
		case 0xAC: { return RES(5, H); } case 0xAD: { return RES(5, L); } case 0xAE: { return RESM(5, get16(HL)); } case 0xAF: { return RES(5, A); }
		case 0xB0: { return RES(6, B); } case 0xB1: { return RES(6, C); } case 0xB2: { return RES(6, D); } case 0xB3: { return RES(6, E); }
		case 0xB4: { return RES(6, H); } case 0xB5: { return RES(6, L); } case 0xB6: { return RESM(6, get16(HL)); } case 0xB7: { return RES(6, A); }
		case 0xB8: { return RES(7, B); } case 0xB9: { return RES(7, C); } case 0xBA: { return RES(7, D); } case 0xBB: { return RES(7, E); }
		case 0xBC: { return RES(7, H); } case 0xBD: { return RES(7, L); } case 0xBE: { return RESM(7, get16(HL)); } case 0xBF: { return RES(7, A); }
				 // Bit Sets
		case 0xC0: { return SET(0, B); } case 0xC1: { return SET(0, C); } case 0xC2: { return SET(0, D); } case 0xC3: { return SET(0, E); }
		case 0xC4: { return SET(0, H); } case 0xC5: { return SET(0, L); } case 0xC6: { return SETM(0, get16(HL)); } case 0xC7: { return SET(0, A); }
		case 0xC8: { return SET(1, B); } case 0xC9: { return SET(1, C); } case 0xCA: { return SET(1, D); } case 0xCB: { return SET(1, E); }
		case 0xCC: { return SET(1, H); } case 0xCD: { return SET(1, L); } case 0xCE: { return SETM(1, get16(HL)); } case 0xCF: { return SET(1, A); }
		case 0xD0: { return SET(2, B); } case 0xD1: { return SET(2, C); } case 0xD2: { return SET(2, D); } case 0xD3: { return SET(2, E); }
		case 0xD4: { return SET(2, H); } case 0xD5: { return SET(2, L); } case 0xD6: { return SETM(2, get16(HL)); } case 0xD7: { return SET(2, A); }
		case 0xD8: { return SET(3, B); } case 0xD9: { return SET(3, C); } case 0xDA: { return SET(3, D); } case 0xDB: { return SET(3, E); }
		case 0xDC: { return SET(3, H); } case 0xDD: { return SET(3, L); } case 0xDE: { return SETM(3, get16(HL)); } case 0xDF: { return SET(3, A); }
		case 0xE0: { return SET(4, B); } case 0xE1: { return SET(4, C); } case 0xE2: { return SET(4, D); } case 0xE3: { return SET(4, E); }
		case 0xE4: { return SET(4, H); } case 0xE5: { return SET(4, L); } case 0xE6: { return SETM(4, get16(HL)); } case 0xE7: { return SET(4, A); }
		case 0xE8: { return SET(5, B); } case 0xE9: { return SET(5, C); } case 0xEA: { return SET(5, D); } case 0xEB: { return SET(5, E); }
		case 0xEC: { return SET(5, H); } case 0xED: { return SET(5, L); } case 0xEE: { return SETM(5, get16(HL)); } case 0xEF: { return SET(5, A); }
		case 0xF0: { return SET(6, B); } case 0xF1: { return SET(6, C); } case 0xF2: { return SET(6, D); } case 0xF3: { return SET(6, E); }
		case 0xF4: { return SET(6, H); } case 0xF5: { return SET(6, L); } case 0xF6: { return SETM(6, get16(HL)); } case 0xF7: { return SET(6, A); }
		case 0xF8: { return SET(7, B); } case 0xF9: { return SET(7, C); } case 0xFA: { return SET(7, D); } case 0xFB: { return SET(7, E); }
		case 0xFC: { return SET(7, H); } case 0xFD: { return SET(7, L); } case 0xFE: { return SETM(7, get16(HL)); } case 0xFF: { return SET(7, A); }
	}
	return 4;
}