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

void CPU::initInst() {
	// Control Instructions
	inst[0x00] = &NOP; inst[0x10] = &STOP;
	inst[0xC8] = &PREF; inst[0xF3] = &DI;
	inst[0xF8] = &EI;

	// Jumps / Calls
	for (byte i = 0x18; i < 0x39; i += 0x8) { inst[i] = &JR; }
	for (byte i = 0xC0; i < 0xD7; i += 0x8) { inst[i] = &RET; }
	inst[0xC9] = &RET;
	for (byte i = 0xC2; i < 0xDB; i += 0x8) { inst[i] = &JP; }
	inst[0xC3] = &JP; inst[0xE9] = &JP;
	for (byte i = 0xC4; i < 0xDD; i += 0x8) { inst[i] = &CALL; }
	inst[0xCD] = &CALL;
	for (byte i = 0xC7; i <= 0xFF; i += 0x8) { inst[i] = &RST; }

	// Load Instructions
	for (byte i = 0x02; i < 0x3F; i += 0x4) { inst[i] = &LD; }
	for (byte i = 0x40; i < 0x80; i++) {
		if (i == 0x76) {
			inst[i] = &HALT;
		}
		inst[i] = &LD;
	}

	// 16 bit Load Instructions
	for (byte i = 0x01; i < 0x32; i += 0xF) { inst[i] = &LD16; }
	inst[0x08] = &LD16;
	for (byte i = 0xC1; i < 0xF6; i += 0xB) {
		inst[i] = &POP; i+=4;
		inst[i] = &PUSH;
	}
	inst[0xF8] = &LD16; inst[0xF9] = &LD16;

	// Arithmetic Instructions
	for (byte i = 0x04; i < 0x3D; i += 0xF) { inst[i] = &INC; }
	for (byte i = 0x05; i < 0x3E; i += 0xF) { inst[i] = &DEC; }
	inst[0x2F] = &CPL; inst[0x3F] = &CCF;
	for (byte i = 0x80; i < 0xC0; i++) {
		if (i < 0x88) { inst[i] = &ADD; continue; }
		if (i < 0x90) { inst[i] = &ADC; continue; }
		if (i < 0x98) { inst[i] = &SUB; continue; }
		if (i < 0xA0) { inst[i] = &SBC; continue; }
		if (i < 0xA8) { inst[i] = &AND; continue; }
		if (i < 0xB0) { inst[i] = &XOR; continue; }
		if (i < 0xB8) { inst[i] = &OR; continue; }
		inst[i] = &CP;
	}
	inst[0x27] = &DAA; inst[0x2F] = &CPL;
	inst[0x37] = &SCF; inst[0x3F] = &CCF;
	inst[0xC6] = &ADD; inst[0xCE] = &ADC;
	inst[0xD6] = &SUB; inst[0xDE] = &SBC;
	inst[0xE6] = &AND; inst[0xEE] = &XOR;
	inst[0xF6] = &OR; inst[0xFE] = &CP;

	// 16 bit Arithmetic
	for (byte i = 0x03; i < 0x3C; i += 0xF) { inst[i] = &INC16; }
	for (byte i = 0x09; i < 0x3A; i += 0xF) { inst[i] = &ADD16; }
	for (byte i = 0x0B; i < 0x3C; i += 0xF) { inst[i] = &DEC16; }
	inst[0xE8] = &ADD16;
}

// Instructions
// ------------

// Load
void CPU::LD(word op) { printf("LD: %X", op); }

void CPU::LD16(word op) { printf("LD16: %X", op); }

void CPU::POP(word op) { printf("POP: %X", op); }

void CPU::PUSH(word op) { printf("PUSH: %X", op); }

// Arithmetic
void CPU::ADD(word op) { printf("ADD: %X", op); }

void CPU::ADC(word op) { printf("ADC: %X", op); }

void CPU::ADD16(word op) { printf("ADD16: %X", op); }

void CPU::SUB(word op) { printf("SUB: %X", op); }

void CPU::SBC(word op) { printf("SBC: %X", op); }

void CPU::AND(word op) { printf("AND: %X", op); }

void CPU::XOR(word op) { printf("XOR: %X", op); }

void CPU::OR(word op) { printf("OR: %X", op); }

void CPU::CP(word op) { printf("CP: %X", op); }

void CPU::INC(word op) { printf("INC: %X", op); }

void CPU::INC16(word op) { printf("INC16: %X", op); }

void CPU::DEC(word op) { printf("DEC: %X", op); }

void CPU::DEC16(word op) { printf("DEC16: %X", op); }

void CPU::CPL(word op) { printf("CPL: %X", op); }

void CPU::CCF(word op) { printf("CCF: %X", op); }

void CPU::DAA(word op) { printf("DAA: %X", op); }

void CPU::SCF(word op) { printf("SCF: %X", op); }

// Control
void CPU::NOP(word op) { printf("NOP: %X", op); }

void CPU::STOP(word op) { printf("STOP: %X", op); }

void CPU::HALT(word op) { printf("HALT: %X", op); }

void CPU::PREF(word op) { printf("PREF: %X", op); }

void CPU::DI(word op) { printf("DI: %X", op); }

void CPU::EI(word op) { printf("EI: %X", op); }

// Jump / Call
void CPU::JR(word op) { printf("JR: %X", op); }

void CPU::JP(word op) { printf("JP: %X", op); }

void CPU::CALL(word op) { printf("CALL: %X", op); }

void CPU::RST(word op) { printf("RST: %X", op); }

void CPU::RET(word op) { printf("RET: %X", op); }

void CPU::RETI(word op) { printf("RETI: %X", op); }
// ------------