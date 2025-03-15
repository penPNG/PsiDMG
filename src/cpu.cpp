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

byte CPU::getZ() { return registers.reg8[F] >> 7; }
byte CPU::getN() { return (registers.reg8[F] & 0b01000000) >> 6; }
byte CPU::getH() { return (registers.reg8[F] & 0b00100000) >> 5; }
byte CPU::getC() { return (registers.reg8[F] & 0b00010000) >> 4; }

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

// Load standard
byte CPU::LD(reg8 x, reg8 y) { 
	printf("LDH: %d %d", x, y);

	set8(x, get8(y));
	return 4;
}

// Load Immediate to Register
byte CPU::LDI(reg8 reg, byte b) { 
	printf("LDI: %d %d", reg, b); 

	set8(reg, b);
	return 8;
}

// Load Immediate to Memory
byte CPU::LDMI(word addr, byte b) {
	printf("LDMI: %d %d", addr, b);

	setRam(addr, b);
	return 12;
}

// Load to Register from Memory
byte CPU::LDRM(reg8 reg, word addr) {
	printf("LDRM: %d %d", reg, addr);

	set8(reg, ram.ram[addr]);
	return 8;
}

// Load to Memory from Register
byte CPU::LDMR(word addr, reg8 reg) {
	printf("LDMR: %d %d", addr, reg);

	ram.ram[addr] = get8(reg);
	return 8;
}

// Load 16 Bit Immediate
byte CPU::LD16(reg16 reg, word w) {
	printf("LD16: %d %d", reg, w);

	set16(reg, w);
	return 12;
}

// Load to SP from HL
byte CPU::LD16SPHL() {
	printf("LD16SPHL: %d %d", get16(SP), get16(HL));

	set16(SP, get16(HL));
	return 8;
}

// Load to HL from SP + Signed Byte
byte CPU::LD16HLSP(sbyte sb) {
	byte sp;
	printf("LD16HLSP: %d %d %d", get16(HL), sp = get16(SP), sb);

	word res = sp+sb;
	setZ(0); setN(0);
	setH(((sp&0x0F)+(sb&0x0F)) & 0x10);
	setC(res & 0x0100);

	set16(HL, res);
	return 12;
}

// LOAD to Memory from SP
byte CPU::LD16MSP(word addr) {
	printf("LD16HLSP: %d %d", addr, get16(SP));

	ram.ram[addr] = get16(SP) & 0x00FF;
	ram.ram[addr+1] = get16(SP) & (0xFF00 >> 8);
	return 20;
}

// POP Stack to 16 Bit Register
byte CPU::POP(reg16 reg) {
	printf("POP: %d", reg);
	
	set16(reg, ram.ram[registers.reg16[SP]++] | (ram.ram[registers.reg16[SP]++] << 8));
	return 12;
}

// Push 16 Bit Register to Stack
byte CPU::PUSH(reg16 reg) {
	printf("PUSH: %d", reg);

	setRam(--registers.reg16[SP], get16(reg) & (0xFF00 >> 8));
	setRam(--registers.reg16[SP], get16(reg) & 0x00FF);
	return 16;
}
// ------------

// Arithmetic
// ----------

// ADD Register to A
byte CPU::ADD(reg8 reg) { 
	byte a, n;
	printf("ADD: %d %d", a = get8(A), n = get8(reg));

	word res = a+n;
	setZ(res==0); setN(0);
	setH(((a&0x0F)+(n&0x0F)) & 0x10);
	setC(res & 0x0100);
	set8(A, res);
	return 4;
}

// ADD Memory to A
byte CPU::ADDM(word addr) {
	byte n, a;
	printf("ADDM: %d %d", a = get8(A), n = getRam(addr));

	word res = a+n;
	setZ(res==0); setN(0);
	setH(((a & 0x0F) + (n & 0x0F)) & 0x10);
	setC(res & 0x0100);
	set8(A, res);
	return 8;
}

// ADD Immediate to A
byte CPU::ADDI(byte n) {
	byte a;
	printf("ADDI: %d %d", a = get8(A), n);

	word res = a+n;
	setZ(res == 0); setN(0);
	setH(((a & 0x0F) + (n & 0x0F)) & 0x10);
	setC(res & 0x0100);
	set8(A, res);
	return 8;
}

// ADD with Carry Register to A
byte CPU::ADC(reg8 reg) { 
	byte a, n, c;
	printf("ADC: %d %d %d", a = get8(A), n = get8(reg), c = getC());

	word res = a+n+c;
	setZ(res == 0); setN(0);
	setH(((a & 0x0F) + (n & 0x0F) + c) & 0x10);
	setC(res & 0x0100);
	set8(A, res);
	return 4;
}

// ADD with Carry emory to A
byte CPU::ADCM(word addr) {
	byte a, n, c;
	printf("ADCM: %d %d %d", a = get8(A), n = getRam(addr), c = getC());

	word res = a+n+c; 
	setZ(res == 0); setN(0);
	setH(((a & 0x0F) + (n & 0x0F) + c) & 0x10);
	setC(res & 0x0100);
	set8(A, res);
	return 8;
}

// ADD with Carry Immediate to A
byte CPU::ADCI(byte n) {
	byte a, c;
	printf("ADCM: %d %d %d", a = get8(A), n, c = getC());

	word res = a + n + c;
	setZ(res == 0); setN(0);
	setH(((a & 0x0F) + (n & 0x0F) + c) & 0x10);
	setC(res & 0x0100);
	set8(A, res);
	return 8;
}

// ADD 16 Bit Register to HL
byte CPU::ADD16(reg16 reg) { 
	word a, n;
	printf("ADD16: %d %d %d", a = get16(HL), n = get16(reg)); 

	unsigned int res = a + n;
	setN(0);
	setH((a & 0xFF) + (n & 0xFF) & 0x0100);
	setC(res & 0x10000);
	set16(HL, res);
	return 8;
}

// ADD Signed Byte to SP
byte CPU::ADD16I(sbyte n) {
	word a;
	printf("ADD16: %d %d %d", a = get16(SP), n);

	unsigned int res = a + n;
	setZ(0); setN(0);
	setH((a & 0xFF) + (n & 0xFF) & 0x0100);
	setC(res & 0x10000);
	set16(SP, res);
	return 16;
}

// SUB Register from A
byte CPU::SUB(reg8 reg) {
	byte a, n;
	printf("SUB: %d %d", a = get8(A), n = get8(reg));

	word res = a - n;
	setZ(res==0); setN(1);
	setH((a & 0x0F) < (n & 0x0F));
	setC(a < n);
	set8(A, a - n);
	return 4;
}

// SUB Memory from A
byte CPU::SUBM(word addr) {
	byte a, n;
	printf("SUB: %d %d", a = get8(A), n = getRam(addr));

	word res = a - n;
	setZ(res == 0); setN(1);
	setH((a & 0x0F) < (n & 0x0F));
	setC(a < n);
	set8(A, a - n);
	return 8;
}

// SUB Immediate from A
byte CPU::SUBI(byte n) {
	byte a;
	printf("SUB: %d %d", a = get8(A), n);

	word res = a - n;
	setZ(res == 0); setN(1);
	setH((a & 0x0F) < (n & 0x0F));
	setC(a < n);
	set8(A, a-n);
	return 8;
}

// SUB with Carry Register from A
byte CPU::SBC(reg8 reg) {
	byte a, n, c;
	printf("SBC: %d %d %d", a = get8(A), n = get8(reg), c = getC());

	word res = a - (n+c);
	setZ(res == 0); setN(1);
	setH((a & 0x0F) < (n & 0x0F)+c);
	setC(a < n+c);
	set8(A, a - (n+c));
	return 4;
}

// SUB with Carry Memory from A
byte CPU::SBCM(word addr) {
	byte a, n, c;
	printf("SBCM: %d %d %d", a = get8(A), n = getRam(addr), c = getC());

	word res = a - (n + c);
	setZ(res == 0); setN(1);
	setH((a & 0x0F) < (n & 0x0F) + c);
	setC(a < n + c);
	set8(A, a - (n + c));
	return 8;
}

// SUB with Carry Immediate from A
byte CPU::SBCI(byte n) {
	byte a, c;
	printf("SBCI: %d %d %d", a = get8(A), n, c = getC());

	word res = a - (n + c);
	setZ(res == 0); setN(1);
	setH((a & 0x0F) < (n & 0x0F) + c);
	setC(a < n + c);
	set8(A, a - (n + c));
	return 8;
}

// AND Register with A
byte CPU::AND(reg8 reg) {
	byte a, n;
	printf("AND: %d %d", a = get8(A), n = get8(reg));

	byte res = a & n;
	setZ(res == 0); setN(0);
	setH(1); setC(0);
	set8(A, res);
	return 4;
}

// AND Memory with A
byte CPU::ANDM(word addr) {
	byte n, a;
	printf("ANDM: %d %d", a = get8(A), n = getRam(addr));

	byte res = a & n;
	setZ(res == 0); setN(0);
	setH(1); setC(0);
	set8(A, res);
	return 8;
}

// AND Immediate with A
byte CPU::ANDI(byte n) {
	byte a;
	printf("ANDI: %d %d", a = get8(A), n);

	byte res = a & n;
	setZ(res == 0); setN(0);
	setH(1); setC(0);
	set8(A, res);
	return 8;
}

// XOR Register with A
byte CPU::XOR(reg8 reg) {
	byte a, n;
	printf("XOR: %d %d", a = get8(A), n = get8(reg));

	byte res = a ^ n;
	setZ(res == 0); setN(0);
	setH(0); setC(0);
	set8(A, res);
	return 4;
}

// XOR Memory with A
byte CPU::XORM(word addr) {
	byte n, a;
	printf("XORM: %d %d", a = get8(A), n = getRam(addr));

	byte res = a ^ n;
	setZ(res == 0); setN(0);
	setH(0); setC(0);
	set8(A, res);
	return 8;
}

// XOR Immediate with A
byte CPU::XORI(byte n) {
	byte a;
	printf("XORI: %d %d", a = get8(A), n);

	byte res = a ^ n;
	setZ(res == 0); setN(0);
	setH(0); setC(0);
	set8(A, res);
	return 8;
}

// OR Register with A
byte CPU::OR(reg8 reg) {
	byte a, n;
	printf("ADD: %d %d", a = get8(A), n = get8(reg));

	byte res = a | n;
	setZ(res == 0); setN(0);
	setH(0); setC(0);
	set8(A, res);
	return 4;
}

// OR Memory with A
byte CPU::ORM(word addr) {
	byte n, a;
	printf("ADDM: %d %d", a = get8(A), n = getRam(addr));

	byte res = a | n;
	setZ(res == 0); setN(0);
	setH(0); setC(0);
	set8(A, res);
	return 8;
}

// OR Immediate with A
byte CPU::ORI(byte n) {
	byte a;
	printf("ADDI: %d %d", a = get8(A), n);

	byte res = a | n;
	setZ(res == 0); setN(0);
	setH(0); setC(0);
	set8(A, res);
	return 8;
}

// Compare Register with A for equality
byte CPU::CP(reg8 reg) {
	byte a, n;
	printf("CP: %d %d", a = get8(A), n = get8(reg));

	word res = a - n;
	setZ(res == 0); setN(1);
	setH((a & 0x0F) < (n & 0x0F));
	setC(a < n);
	return 4;
}

// Compare Memory with A for equality
byte CPU::CPM(word addr) {
	byte a, n;
	printf("CPM: %d %d", a = get8(A), n = getRam(addr));

	word res = a - n;
	setZ(res == 0); setN(1);
	setH((a & 0x0F) < (n & 0x0F));
	setC(a < n);
	return 8;
}

// Compare Immediate with A for equality
byte CPU::CPI(byte n) {
	byte a;
	printf("CPI: %d %d", a = get8(A), n);

	word res = a - n;
	setZ(res == 0); setN(1);
	setH((a & 0x0F) < (n & 0x0F));
	setC(a < n);
	return 8;
}

// Increment Register
byte CPU::INC(reg8 reg) {
	byte a;
	printf("INC: %d", a = get8(reg));

	setZ(a + 1 == 0); setN(0);
	setH(((a & 0x0F) + 1) & 0x10);
	set8(reg, a+1);
	return 4;
}

// Increment Memory
byte CPU::INCM(word addr) {
	byte a;
	printf("INC: %d", a = getRam(addr));

	setZ(a + 1 == 0); setN(0);
	setH(((a & 0x0F) + 1) & 0x10);
	setRam(addr, a + 1);
	return 12;
}

// Increment 16 Bit Register
byte CPU::INC16(reg16 reg) {
	word a;
	printf("INC16: %d", a = get16(reg));

	set16(reg, a+1);
	return 8;
}

// Decrement Register
byte CPU::DEC(reg8 reg) {
	byte a;
	printf("INC: %d", a = get8(reg));

	setZ(a - 1 == 0); setN(1);
	setH((a & 0x0F) == 0x00);
	set8(reg, a - 1);
	return 4;
}

// Decrement Memory
byte CPU::DECM(word addr) {
	byte a;
	printf("INC: %d", a = getRam(addr));

	setZ(a - 1 == 0); setN(1);
	setH((a & 0x0F) == 0x00);
	setRam(addr, a - 1);
	return 12;
}

// Decrement 16 Bit Register
byte CPU::DEC16(reg16 reg) {
	word a;
	printf("INC16: %d", a = get16(reg));

	set16(reg, a - 1);
	return 8;
}

// One's complement Register A
byte CPU::CPL() {
	byte a;
	printf("CPL: %d", a = get8(A)); 

	setN(1); setH(1);
	set8(A, ~a);
	return 4;
}

// Flip Carry Flag
byte CPU::CCF() { 
	byte c;
	printf("CCF: %d", c = getC()); 

	setC(!c);
	return 4;
}

// Adjust BCD of register A
byte CPU::DAA() {
	byte a, c, h, n;
	printf("DAA: %x", a = get8(A));

	byte correction = 0;
	c = getC(); h = getH(); n = getN();
	if (!n) {
		if (c || ( a > 0x99)) { a += 0x60; setC(1); }
		if (h || (a&0x0f) > 0x09) { a += 0x6; }
	} else {
		if (c) { a -= 0x60; }
		if (h) { a -= 0x6; }
	}
	setZ(a==0);
	setH(0);
	return 4;
}

// Set Carry Flag
byte CPU::SCF() { 
	printf("SCF: %d", getC());

	setN(0); setH(0); setC(1);
	return 4;
}
// ----------

// Control
// -------
byte CPU::NOP(word op) { printf("NOP : 0"); return 4; }

byte CPU::STOP(word op) { printf("STOP"); return 4; }

byte CPU::HALT(word op) { printf("HALT"); return 4; }

byte CPU::PREF(byte ins) {
	if (ins == 0) printf("PREF");
	return 4;
}

byte CPU::DI(word op) { 
	printf("DI");
	return 4;
}

byte CPU::EI(word op) { 
	printf("EI");
	return 4;
}
// -------

// Jump / Call
// -----------
byte CPU::JR(sbyte n) { 
	printf("JR: %d", n); 

}

byte CPU::JP(word op) { printf("JP"); }

byte CPU::CALL(word op) { printf("CALL"); }

byte CPU::RST(word op) { printf("RST"); }

byte CPU::RET(word op) { printf("RET"); }

byte CPU::RETI(word op) { printf("RETI"); }

byte CPU::no(word op) { printf("NOP"); }

//Bit Manipulation
byte CPU::RLCA(word op) { printf("RLCA"); }

byte CPU::RLA(word op) { printf("RLA"); }

byte CPU::RRCA(word op) {printf("RRCA"); }

byte CPU::RRA(word op) {printf("RRA"); }

// CB Instructions
byte CPU::RLC(word op) { printf("RLC"); }

byte CPU::RRC(word op) { printf("RRC"); }

byte CPU::RL(word op) { printf("RL"); }

byte CPU::RR(word op) { printf("RR"); }

byte CPU::SLA(word op) { printf("SLA"); }

byte CPU::SRA(word op) { printf("SRA"); }

byte CPU::SWAP(word op) { printf("SWAP"); }

byte CPU::SRL(word op) { printf("SRL"); }

byte CPU::BIT(word op) { printf("BIT"); }

byte CPU::RES(word op) { printf("RES"); }

byte CPU::SET(word op) { printf("SET"); }

// ------------

// TODO make safer functions!
byte CPU::getRam(word addr) {
	return ram.ram[addr];
}

void CPU::setRam(word addr, byte data) {
	ram.ram[addr] = data;
}