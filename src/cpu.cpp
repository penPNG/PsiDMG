#include "cpu.h"

CPU::CPU() {
	

	registers.regAF.lo = 0x01; registers.regAF.hi = FLAG_Z;
	registers.regBC.reg = 0x0013;
	registers.regDE.reg = 0x00D8;
	registers.regHL.reg = 0x014D;
	registers.regPC.reg = 0x0100;
	registers.regSP.reg = 0xFFFE;
}

// Reigster Functions
// ------------------
byte CPU::getA() { return registers.regAF.lo; }
byte CPU::getF() { return registers.regAF.hi; }
word CPU::getAF() {return registers.regAF.reg; }
byte CPU::getB() { return registers.regBC.lo; }
byte CPU::getC() { return registers.regBC.hi; }
word CPU::getBC() { return registers.regBC.reg; }
byte CPU::getD() { return registers.regDE.lo; }
byte CPU::getE() { return registers.regDE.hi; }
word CPU::getDE() { return registers.regDE.reg; }
byte CPU::getH() { return registers.regHL.lo; }
byte CPU::getL() { return registers.regHL.hi; }
word CPU::getHL() { return registers.regHL.reg; }
word CPU::getSP() { return registers.regSP.reg; }
word CPU::getPC() { return registers.regPC.reg; }

void CPU::setA(byte d) { registers.regAF.lo = d; }
void CPU::setF(byte d) { d &= 0xF0; registers.regAF.hi &= d; } // Set Flags
void CPU::setAF(word d) { d &= 0xFFF0; registers.regAF.reg = d; }
void CPU::setAF(byte l, byte h) { h &= 0xF0; registers.regAF.reg = (h & (l<<8)); }
void CPU::setB(byte d) { registers.regBC.lo = d; }
void CPU::setC(byte d) { registers.regBC.hi = d; }
void CPU::setBC(word d) { registers.regBC.reg = d; }
void CPU::setBC(byte l, byte h) { registers.regBC.reg = (h & (l<<8)); }
void CPU::setD(byte d) { registers.regDE.lo = d; }
void CPU::setE(byte d) { registers.regDE.hi = d; }
void CPU::setDE(word d) { registers.regDE.reg = d; }
void CPU::setDE(byte l, byte h) { registers.regDE.reg = (h & (l<<8)); }
void CPU::setH(byte d) { registers.regHL.lo = d; }
void CPU::setL(byte d) { registers.regHL.hi = d; }
void CPU::setHL(word d) { registers.regHL.reg = d; }
void CPU::setHL(byte l, byte h) { registers.regHL.reg = (h & (l<<8)); }
void CPU::setSP(word d) { registers.regSP.reg = d; }
void CPU::setPC(word d) { registers.regPC.reg = d; }	// Set Program Counter
void CPU::setPCLo(byte d) { registers.regPC.lo = d; }	// Set Lo byte of PC
void CPU::setPCHi(byte d) { registers.regPC.hi = d; }	// Set Hi byte of PC
// -------------------

constexpr void CPU::initInst() {
	// Control Instructions
	inst[0x00] = NOP; inst[0x10] = STOP;
	inst[0xC8] = PREF; inst[0xF3] = DI;
	inst[0xF8] = EI;

	// Jumps / Calls
	for (byte i = 0x18; i < 0x39; i += 0x8) { inst[i] = JR; }
	for (byte i = 0xC0; i < 0xD7; i += 0x8) { inst[i] = RET; }
	inst[0xC9] = RET; inst[0xD9] = RETI;
	for (byte i = 0xC2; i < 0xDB; i += 0x8) { inst[i] = JP; }
	inst[0xC3] = JP; inst[0xE9] = JP;
	for (byte i = 0xC4; i < 0xDD; i += 0x8) { inst[i] = CALL; }
	inst[0xCD] = CALL;
	for (byte i = 0xC7; i <= 0xFF; i += 0x8) { inst[i] = RST; }

	// Load Instructions
	for (byte i = 0x02; i < 0x3F; i += 0x4) { inst[i] = LD; }
	for (byte i = 0x40; i < 0x80; i++) {
		if (i == 0x76) {
			inst[i] = HALT;
		}
		inst[i] = LD;
	}

	// 16 bit Load Instructions
	for (byte i = 0x01; i < 0x32; i += 0xF) { inst[i] = LD16; }
	inst[0x08] = LD16;
	for (byte i = 0xC1; i < 0xF6; i += 0xB) {
		inst[i] = POP; i+=4;
		inst[i] = PUSH;
	}
	inst[0xF8] = LD16; inst[0xF9] = LD16;

	// Arithmetic Instructions
	for (byte i = 0x04; i < 0x3D; i += 0xF) { inst[i] = INC; }
	for (byte i = 0x05; i < 0x3E; i += 0xF) { inst[i] = DEC; }
	inst[0x2F] = CPL; inst[0x3F] = CCF;
	for (byte i = 0x80; i < 0xC0; i++) {
		if (i < 0x88) { inst[i] = ADD; continue; }
		if (i < 0x90) { inst[i] = ADC; continue; }
		if (i < 0x98) { inst[i] = SUB; continue; }
		if (i < 0xA0) { inst[i] = SBC; continue; }
		if (i < 0xA8) { inst[i] = AND; continue; }
		if (i < 0xB0) { inst[i] = XOR; continue; }
		if (i < 0xB8) { inst[i] = OR; continue; }
		inst[i] = CP;
	}
	inst[0x27] = DAA; inst[0x2F] = CPL;
	inst[0x37] = SCF; inst[0x3F] = CCF;
	inst[0xC6] = ADD; inst[0xCE] = ADC;
	inst[0xD6] = SUB; inst[0xDE] = SBC;
	inst[0xE6] = AND; inst[0xEE] = XOR;
	inst[0xF6] = OR; inst[0xFE] = CP;

	// 16 bit Arithmetic
	for (byte i = 0x03; i < 0x3C; i += 0xF) { inst[i] = INC16; }
	for (byte i = 0x09; i < 0x3A; i += 0xF) { inst[i] = ADD16; }
	for (byte i = 0x0B; i < 0x3C; i += 0xF) { inst[i] = DEC16; }
	inst[0xE8] = ADD16;
}