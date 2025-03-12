#include "cpu.h"

CPU::CPU() {
	initInst();

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
	for (word i = 0; i < 256; i++) {
		inst[i] = &no;
	}

	// Control Instructions
	inst[0x00] = &NOP; inst[0x10] = &STOP;
	inst[0xCB] = &PREF; inst[0xF3] = &DI;
	inst[0xFB] = &EI;

	//// Jumps / Calls
	for (word i = 0x18; i < 0x39; i += 0x8) { inst[i] = &JR; }
	for (word i = 0xC0; i < 0xD9; i += 0x8) { inst[i] = &RET; }
	inst[0xC9] = &RET;
	for (word i = 0xC2; i < 0xDB; i += 0x8) { inst[i] = &JP; }
	inst[0xC3] = &JP; inst[0xE9] = &JP;
	for (word i = 0xC4; i < 0xDD; i += 0x8) { inst[i] = &CALL; }
	inst[0xCD] = &CALL;
	for (word i = 0xC7; i <= 0xFF; i += 0x8) { inst[i] = &RST; }

	//// Load Instructions
	for (word i = 0x02; i < 0x3F; i += 0x4) { inst[i] = &LD; }
	for (word i = 0x40; i < 0x80; i++) {
		if (i == 0x76) {
			inst[i] = &HALT;
			continue;
		}
		inst[i] = &LD;
	}
	for (word i = 0xE0; i < 0xF3; i += 0x6) {
		inst[i] = LDH; i += 2;
		inst[i] = LDH; i += 8;
		inst[i] = LD;
	}

	//// 16 bit Load Instructions
	for (word i = 0x01; i < 0x32; i += 0x10) { inst[i] = &LD16; }
	inst[0x08] = &LD16;
	for (word i = 0xC1; i < 0xF6; i += 0xC) {
		inst[i] = &POP; i+=4;
		inst[i] = &PUSH;
	}
	inst[0xF8] = &LD16; inst[0xF9] = &LD16;

	//// Arithmetic Instructions
	for (word i = 0x04; i < 0x3D; i += 0x8) { inst[i] = &INC; }
	for (word i = 0x05; i < 0x3E; i += 0x8) { inst[i] = &DEC; }
	inst[0x2F] = &CPL; inst[0x3F] = &CCF;
	for (word i = 0x80; i < 0xC0; i++) {
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

	//// 16 bit Arithmetic
	for (word i = 0x03; i < 0x3C; i += 0x10) { inst[i] = &INC16; }
	for (word i = 0x09; i < 0x3A; i += 0x10) { inst[i] = &ADD16; }
	for (word i = 0x0B; i < 0x3C; i += 0x10) { inst[i] = &DEC16; }
	inst[0xE8] = &ADD16;

	// Bit manipulation
	inst[0x7] = &RLCA; inst[0xF] = &RRCA;
	inst[0x17] = &RLA; inst[0x1F] = &RRA;

	// CB Instructions
	for (word i = 0x00; i < 0x08; i++) { CBInst[i] = &RLC; }
	for (word i = 0x08; i < 0x10; i++) { CBInst[i] = &RRC; }
	for (word i = 0x10; i < 0x18; i++) { CBInst[i] = &RL; }
	for (word i = 0x18; i < 0x20; i++) { CBInst[i] = &RR; }
	for (word i = 0x20; i < 0x28; i++) { CBInst[i] = &SLA; }
	for (word i = 0x28; i < 0x30; i++) { CBInst[i] = &SRA; }
	for (word i = 0x30; i < 0x38; i++) { CBInst[i] = &SWAP; }
	for (word i = 0x38; i < 0x40; i++) { CBInst[i] = &SRL; }
	for (word i = 0x40; i < 0x80; i++) { CBInst[i] = &BIT; }
	for (word i = 0x80; i < 0xC0; i++) { CBInst[i] = &RES; }
	for (word i = 0xC0; i < 0x100; i++) { CBInst[i] = &SET; }
}

// Instructions
// ------------

// Load
void CPU::LD(word op) { printf("LD"); }

void CPU::LDH(word op) { printf("LDH"); }

void CPU::LD16(word op) { printf("LD16"); }

void CPU::POP(word op) { printf("POP"); }

void CPU::PUSH(word op) { printf("PUSH"); }

// Arithmetic
void CPU::ADD(word op) { printf("ADD"); }

void CPU::ADC(word op) { printf("ADC"); }

void CPU::ADD16(word op) { printf("ADD16"); }

void CPU::SUB(word op) { printf("SUB"); }

void CPU::SBC(word op) { printf("SBC"); }

void CPU::AND(word op) { printf("AND"); }

void CPU::XOR(word op) { printf("XOR"); }

void CPU::OR(word op) { printf("OR"); }

void CPU::CP(word op) { printf("CP"); }

void CPU::INC(word op) { printf("INC"); }

void CPU::INC16(word op) { printf("INC16"); }

void CPU::DEC(word op) { printf("DEC"); }

void CPU::DEC16(word op) { printf("DEC16"); }

void CPU::CPL(word op) { printf("CPL"); }

void CPU::CCF(word op) { printf("CCF"); }

void CPU::DAA(word op) { printf("DAA"); }

void CPU::SCF(word op) { printf("SCF"); }

// Control
void CPU::NOP(word op) { printf("NOP"); }

void CPU::STOP(word op) { printf("STOP"); }

void CPU::HALT(word op) { printf("HALT"); }

void CPU::PREF(word op) {
	if (op == 0x3244) { printf("PREF"); return; }
	CBInst[op & 0x00FF](op);
}

void CPU::DI(word op) { printf("DI"); }

void CPU::EI(word op) { printf("EI"); }

// Jump / Call
void CPU::JR(word op) { printf("JR"); }

void CPU::JP(word op) { printf("JP"); }

void CPU::CALL(word op) { printf("CALL"); }

void CPU::RST(word op) { printf("RST"); }

void CPU::RET(word op) { printf("RET"); }

void CPU::RETI(word op) { printf("RETI"); }

void CPU::no(word op) { printf("NOP"); }

//Bit Manipulation
void CPU::RLCA(word op) { printf("RLCA"); }

void CPU::RLA(word op) { printf("RLA"); }

void CPU::RRCA(word op) {printf("RRCA"); }

void CPU::RRA(word op) {printf("RRA"); }

// CB Instructions
void CPU::RLC(word op) { printf("RLC"); }

void CPU::RRC(word op) { printf("RRC"); }

void CPU::RL(word op) { printf("RL"); }

void CPU::RR(word op) { printf("RR"); }

void CPU::SLA(word op) { printf("SLA"); }

void CPU::SRA(word op) { printf("SRA"); }

void CPU::SWAP(word op) { printf("SWAP"); }

void CPU::SRL(word op) { printf("SRL"); }

void CPU::BIT(word op) { printf("BIT"); }

void CPU::RES(word op) { printf("RES"); }

void CPU::SET(word op) { printf("SET"); }

// ------------