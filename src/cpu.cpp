#include "cpu.h"

CPU::CPU(Memory& _ram) : ram(_ram) {
	initInst();

	registers.reg8[A] = 0x01; registers.reg8[F] = FLAG_Z;
	registers.reg16[BC] = 0x0013;
	registers.reg16[DE] = 0x00D8;
	registers.reg16[HL] = 0x014D;
	registers.reg16[SP] = 0xFFFE;
}

// Reigster Functions
// ------------------
byte CPU::get8(reg8 reg) { return registers.reg8[reg]; }
word CPU::get16(reg16 reg) {return registers.reg16[reg]; }

void CPU::set8(reg8 reg, byte b) { registers.reg8[reg] = b; }
void CPU::set16(reg16 reg, word w) { registers.reg16[reg] = w; }
void CPU::set16(reg16 reg, byte l, byte h) { registers.reg16[reg] = (l | (h << 8)); }

void CPU::setZ(bool z) { if (z) { set8(F, flagZ); } }
void CPU::setN(bool n) { if (n) { set8(F, flagN); } }
void CPU::setH(bool h) { if (h) { set8(F, flagH); } }
void CPU::setC(bool c) { if (c) { set8(F, flagC); } }
// -------------------

void CPU::exec(word op) {
	byte inst = (op & 0xFF00) >> 8;
	byte data = (op & 0xFF);

	switch (inst) {
		// Control Instructions
		case 0x00: { NOP(op); return; } case 0x10: { STOP(op); return; }
		case 0xCB: { PREF(op); return; } case 0xF3: { DI(op); return; }
		case 0xFB: { EI(op); return; }

		// Jumps / Calls
		case 0x18: case 0x20: case 0x28: case 0x30: case 0x38: { JR(op); return; }
		case 0xC0: case 0xC8: case 0xC9: case 0xD0: case 0xD8: { RET(); return; }
		case 0xC2: case 0xC3: case 0xCA: case 0xD2: case 0xDA: { JP(op); return; }
	}
}

void CPU::initInst() {
	//for (word i = 0; i < 256; i++) {
	//	inst[i] = &CPU::no;
	//}
	//
	//// Control Instructions
	//inst[0x00] = &NOP; inst[0x10] = &CPU::STOP;
	//inst[0xCB] = &CPU::PREF; inst[0xF3] = &CPU::DI;
	//inst[0xFB] = &CPU::EI;

	//// Jumps / Calls
	//for (word i = 0x18; i < 0x39; i += 0x8) { inst[i] = &CPU::JR; }
	//for (word i = 0xC0; i < 0xD9; i += 0x8) { inst[i] = &CPU::RET; }
	//inst[0xC9] = &CPU::RET;
	//for (word i = 0xC2; i < 0xDB; i += 0x8) { inst[i] = &CPU::JP; }
	//inst[0xC3] = &CPU::JP; inst[0xE9] = &CPU::JP;
	//for (word i = 0xC4; i < 0xDD; i += 0x8) { inst[i] = &CPU::CALL; }
	//inst[0xCD] = &CPU::CALL;
	//for (word i = 0xC7; i <= 0xFF; i += 0x8) { inst[i] = &CPU::RST; }

	//// Load Instructions
	//for (word i = 0x02; i < 0x3F; i += 0x4) { inst[i] = &CPU::LD; }
	//for (word i = 0x40; i < 0x80; i++) {
	//	if (i == 0x76) {
	//		inst[i] = &CPU::HALT;
	//		continue;
	//	}
	//	inst[i] = &CPU::LD;
	//}
	//for (word i = 0xE0; i < 0xF3; i += 0x6) {
	//	inst[i] = &CPU::LDH; i += 2;
	//	inst[i] = &CPU::LDH; i += 8;
	//	inst[i] = &CPU::LD;
	//}

	////// 16 bit Load Instructions
	//for (word i = 0x01; i < 0x32; i += 0x10) { inst[i] = &CPU::LD16; }
	//inst[0x08] = &CPU::LD16;
	//for (word i = 0xC1; i < 0xF6; i += 0xC) {
	//	inst[i] = &CPU::POP; i+=4;
	//	inst[i] = &CPU::PUSH;
	//}
	//inst[0xF8] = &CPU::LD16; inst[0xF9] = &CPU::LD16;

	////// Arithmetic Instructions
	//for (word i = 0x04; i < 0x3D; i += 0x8) { inst[i] = &CPU::INC; }
	//for (word i = 0x05; i < 0x3E; i += 0x8) { inst[i] = &CPU::DEC; }
	//inst[0x2F] = &CPU::CPL; inst[0x3F] = &CPU::CCF;
	//for (word i = 0x80; i < 0xC0; i++) {
	//	if (i < 0x88) { inst[i] = &CPU::ADD; continue; }
	//	if (i < 0x90) { inst[i] = &CPU::ADC; continue; }
	//	if (i < 0x98) { inst[i] = &CPU::SUB; continue; }
	//	if (i < 0xA0) { inst[i] = &CPU::SBC; continue; }
	//	if (i < 0xA8) { inst[i] = &CPU::AND; continue; }
	//	if (i < 0xB0) { inst[i] = &CPU::XOR; continue; }
	//	if (i < 0xB8) { inst[i] = &CPU::OR; continue; }
	//	inst[i] = &CPU::CP;
	//}
	//inst[0x27] = &CPU::DAA; inst[0x2F] = &CPU::CPL;
	//inst[0x37] = &CPU::SCF; inst[0x3F] = &CPU::CCF;
	//inst[0xC6] = &CPU::ADD; inst[0xCE] = &CPU::ADC;
	//inst[0xD6] = &CPU::SUB; inst[0xDE] = &CPU::SBC;
	//inst[0xE6] = &CPU::AND; inst[0xEE] = &CPU::XOR;
	//inst[0xF6] = &CPU::OR; inst[0xFE] = &CPU::CP;

	////// 16 bit Arithmetic
	//for (word i = 0x03; i < 0x3C; i += 0x10) { inst[i] = &CPU::INC16; }
	//for (word i = 0x09; i < 0x3A; i += 0x10) { inst[i] = &CPU::ADD16; }
	//for (word i = 0x0B; i < 0x3C; i += 0x10) { inst[i] = &CPU::DEC16; }
	//inst[0xE8] = &CPU::ADD16;

	//// Bit manipulation
	//inst[0x7] = &CPU::RLCA; inst[0xF] = &CPU::RRCA;
	//inst[0x17] = &CPU::RLA; inst[0x1F] = &CPU::RRA;

	//// CB Instructions
	//for (word i = 0x00; i < 0x08; i++) { CBInst[i] = &CPU::RLC; }
	//for (word i = 0x08; i < 0x10; i++) { CBInst[i] = &CPU::RRC; }
	//for (word i = 0x10; i < 0x18; i++) { CBInst[i] = &CPU::RL; }
	//for (word i = 0x18; i < 0x20; i++) { CBInst[i] = &CPU::RR; }
	//for (word i = 0x20; i < 0x28; i++) { CBInst[i] = &CPU::SLA; }
	//for (word i = 0x28; i < 0x30; i++) { CBInst[i] = &CPU::SRA; }
	//for (word i = 0x30; i < 0x38; i++) { CBInst[i] = &CPU::SWAP; }
	//for (word i = 0x38; i < 0x40; i++) { CBInst[i] = &CPU::SRL; }
	//for (word i = 0x40; i < 0x80; i++) { CBInst[i] = &CPU::BIT; }
	//for (word i = 0x80; i < 0xC0; i++) { CBInst[i] = &CPU::RES; }
	//for (word i = 0xC0; i < 0x100; i++) { CBInst[i] = &CPU::SET; }
}

// Instructions
// ------------
void CPU::LD(reg8 x, reg8 y) { 
	printf("LDH: %d %d", x, y);

	set8(x, get8(y));
}

void CPU::LDI(reg8 reg, byte b) { 
	printf("LDI: %d %d", reg, b); 

	set8(reg, b);
}

void CPU::LDRM(reg8 reg, word addr) {
	printf("LDRM: %d %d", reg, addr);

	set8(reg, ram.ram[addr]);
}

void CPU::LDMR(word addr, reg8 reg) {
	printf("LDMR: %d %d", addr, reg);

	ram.ram[addr] = get8(reg);
}

void CPU::LD16(reg16 reg, word w) {
	printf("LD16: %d %d", reg, w);

	set16(reg, w);
}

void CPU::LD16SPHL() {
	printf("LD16SPHL: %d %d", get16(SP), get16(HL));

	set16(SP, get16(HL));
}

void CPU::LD16HLSP(sbyte sb) {
	printf("LD16HLSP: %d %d %d", get16(HL), get16(SP), sb);

	set16(HL, get16(SP)+sb);
}

void CPU::LD16MSP(word addr) {
	printf("LD16HLSP: %d %d", addr, get16(SP));

	ram.ram[addr] = get16(SP) & 0x00FF;
	ram.ram[addr+1] = get16(SP) & (0xFF00 >> 8);
}

void CPU::POP(reg16 reg) {
	printf("POP: %d", reg);
	
	set16(reg, ram.ram[registers.reg16[SP]++] | (ram.ram[registers.reg16[SP]++] << 8));
}

void CPU::PUSH(reg16 reg) {
	printf("PUSH: %d", reg);

	setRam(--registers.reg16[SP], get16(reg) & (0xFF00 >> 8));
	setRam(--registers.reg16[SP], get16(reg) & 0x00FF);
}
// ------------

// Arithmetic
// ----------
void CPU::ADD(reg8 reg) { 
	byte a, n;
	printf("ADD: %d %d", a = get8(A), n = get8(reg));
	word res = a+n;
	
	setZ(res==0); setN(false);
	setH(((a&0x0F)+(n&0x0F)) & 0x10);
	setC(res & 0x0100);
	set8(A, res);
}

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
// ----------

// Control
// -------
void CPU::NOP(word op) { printf("NOP"); }

void CPU::STOP(word op) { printf("STOP"); }

void CPU::HALT(word op) { printf("HALT"); }

void CPU::PREF(word op) {
	if (op == 0x3244) { printf("PREF"); return; }
	//CBInst[op & 0x00FF](op);
}

void CPU::DI(word op) { printf("DI"); }

void CPU::EI(word op) { printf("EI"); }
// -------

// Jump / Call
// -----------
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

// TODO make safer functions!
byte CPU::getRam(word addr) {
	return ram.ram[addr];
}

void CPU::setRam(word addr, byte data) {
	ram.ram[addr] = data;
}