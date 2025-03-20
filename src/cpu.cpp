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
	byte a, n; bool c;
	printf("ADC: %d %d %d", a = get8(A), n = get8(reg), c = getC());

	word res = c ? a+n+1 : a+n;
	setZ(res == 0); setN(0);
	setH(((a & 0x0F) + (n & 0x0F) + c) & 0x10);
	setC(res & 0x0100);
	set8(A, res);
	return 4;
}

// ADD with Carry emory to A
byte CPU::ADCM(word addr) {
	byte a, n; bool c;
	printf("ADCM: %d %d %d", a = get8(A), n = getRam(addr), c = getC());

	word res = c ? a + n + 1 : a + n;
	setZ(res == 0); setN(0);
	setH(((a & 0x0F) + (n & 0x0F) + c) & 0x10);
	setC(res & 0x0100);
	set8(A, res);
	return 8;
}

// ADD with Carry Immediate to A
byte CPU::ADCI(byte n) {
	byte a; bool c;
	printf("ADCM: %d %d %d", a = get8(A), n, c = getC());

	word res = c ? a + n + 1 : a + n;
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
	byte a, n; bool c;
	printf("SBC: %d %d %d", a = get8(A), n = get8(reg), c = getC());

	word res = c ? a - (n+1) : a - n;
	setZ(res == 0); setN(1);
	setH((a & 0x0F) < (n & 0x0F)+c);
	setC(a < n+c);
	set8(A, a - (n+c));
	return 4;
}

// SUB with Carry Memory from A
byte CPU::SBCM(word addr) {
	byte a, n; bool c;
	printf("SBCM: %d %d %d", a = get8(A), n = getRam(addr), c = getC());

	word res = c ? a - (n + 1) : a - n;
	setZ(res == 0); setN(1);
	setH((a & 0x0F) < (n & 0x0F) + c);
	setC(a < n + c);
	set8(A, a - (n + c));
	return 8;
}

// SUB with Carry Immediate from A
byte CPU::SBCI(byte n) {
	byte a; bool c;
	printf("SBCI: %d %d %d", a = get8(A), n, c = getC());

	word res = c ? a - (n + 1) : a - n;
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
	bool c;
	printf("CCF: %d", c = getC()); 

	setC(!c);
	return 4;
}

// Adjust BCD of register A
byte CPU::DAA() {
	byte a; bool c, h, n;
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

// Advance PC by one
byte CPU::NOP(word op) { printf("NOP : 0"); return 4; }

// Stop the clock... chaos ensues
byte CPU::STOP(word op) { printf("STOP"); return 4; }

// A special tool we're saving for later
byte CPU::HALT(word op) { printf("HALT"); return 4; }

// Prefix for CB instructions
byte CPU::PREF(byte ins) {
	if (ins == 0) printf("PREF");
	return 4;
}

// Disable Interupts
byte CPU::DI(word op) { 
	printf("DI");
	IME = 0;
	return 4;
}

// Enable Interupts
byte CPU::EI(word op) { 
	printf("EI");
	IME = 1;
	return 4;
}
// -------

// Jump / Call
// -----------

// Jump the value of the Signed Byte
byte CPU::JR(sbyte n) { 
	printf("JR: %d", n);
	PC += n;
	return 8;
}

// Jump the value of the Signed Byte if Flag is set
byte CPU::JRS(flag f, sbyte n) {
	bool F;
	switch (f) {
		case flagZ: F = getZ(); break;
		case flagC: F = getC(); break;
	}
	printf("JRS: %d %d", F, n);
	if (F) { PC += n; }
	return 8;
}

// Jump the value of the Signed Byte if Flag is not set
byte CPU::JRN(flag f, sbyte n) {
	bool F;
	switch (f) {
		case flagZ: F = getZ(); break; 
		case flagC: F = getC(); break;
	}
	printf("JRN: %d %d", F, n);
	if (!F) { PC += n; }
	return 8;
}

// Jump to given Address
byte CPU::JP(word addr) {
	printf("JP: %X", addr); 
	PC = addr;
	return 12;
}

// Jump to HL
byte CPU::JPHL() {
	printf("JPHL: %X", get16(HL));
	PC = get16(HL);
	return 4;
}

// Jump to given Address if Flag is set
byte CPU::JPS(flag f, word addr) {
	bool F;
	switch (f) {
		case flagZ: F = getZ(); break;
		case flagC: F = getC(); break;
	}
	printf("JPS: %d %X", F, addr);
	if (F) { PC = addr; }
	return 8;
}

// Jump to given Address if Flag is not set
byte CPU::JPN(flag f, word addr) {
	bool F;
	switch (f) {
		case flagZ: F = getZ(); break;
		case flagC: F = getC(); break;
	}
	printf("JPN: %d %X", F, addr);
	if (!F) { PC = addr; }
	return 8;
}

// Call given Address
byte CPU::CALL(word addr) { 
	printf("CALL: %X", addr);
	push(PC);
	PC = addr;
	return 12;
}

// Call given Address if Flag is set
byte CPU::CLLS(flag f, word addr) {
	bool F;
	switch (f) {
		case flagZ: F = getZ(); break;
		case flagC: F = getC(); break;
	}
	printf("CALL: %d %X", F, addr);
	if (F) { push(PC); PC = addr; }
	return 12;
}

// Call given Address if Flag is not set
byte CPU::CLLN(flag f, word addr) {
	bool F;
	switch (f) {
		case flagZ: F = getZ(); break;
		case flagC: F = getC(); break;
	}
	printf("CALL: %d %X", F, addr);
	if (!F) { push(PC); PC = addr; }
	return 12;
}

// Push PC to Stack, Jump to $0000 + Byte
byte CPU::RST(byte n) {
	printf("RST: %X", n);
	push(PC);
	PC = n;
	return 32;
}

// Pop Stack to PC
byte CPU::RET() {
	printf("RET: %X", PC);
	PC = pop();
	return 8;
}

// Pop Stack to PC if Flag is set
byte CPU::RTS(flag f) {
	bool F;
	switch (f) {
		case flagZ: F = getZ(); break;
		case flagC: F = getC(); break;
	}
	printf("RTS: %d %X", F, PC);
	if (F) { PC = pop(); }
	return 8;
}

// Pop Stack to PC if Flag is not set
byte CPU::RTN(flag f) {
	bool F;
	switch (f) {
		case flagZ: F = getZ(); break;
		case flagC: F = getC(); break;
	}
	printf("RTN: %d %X", F, PC);
	if (!F) { PC = pop(); }
	return 8;
}

// Pop Stack to PC and Enable Interupts
byte CPU::RETI() {
	printf("RETI: %X", PC);
	PC = pop();
	IME = 1;
	return 8;
}
// -----------

// Bit Manipulation
// ----------------

// Rotate A Left with Carry
byte CPU::RLCA() {
	byte a;
	printf("RLCA: %x", a = get8(A));
	setZ(a == 0); setN(0); setH(0);
	setC(a & 0x80);
	set8(A, (a << 1) | (a >> 7));
	return 4;
}

// Rotate A Left through Carry
byte CPU::RLA() { 
	byte a; bool c;
	printf("RLA: %x %d", a = get8(A), c = getC());
	setZ(a == 0); setN(0); setH(0);
	setC(a & 0x80);
	set8(A, (a << 1) | c);
	return 4;
}

// Rotate A Right with Carry
byte CPU::RRCA() {
	byte a;
	printf("RRCA: %x", a = get8(A));
	setZ(a == 0); setN(0); setH(0);
	setC(a & 0x01);
	set8(A, (a >> 1) | (a << 7));
	return 4;
}

// Rotate A Right through Carry
byte CPU::RRA() {
	byte a; bool c;
	printf("RRA: %x %d", a = get8(A), c = getC());
	setZ(a == 0); setN(0); setH(0);
	setC(a & 0x01);
	set8(A, (a >> 1) | (c << 7));
	return 4;
}
// ----------------

// CB Instructions
// ---------------

// Rotate Register Left with Carry
byte CPU::RLC(reg8 reg) { 
	byte n;
	printf("RLC: %x", n = get8(reg));
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x80);
	set8(reg, (n << 1) | (n >> 7));
	return 8;
}

// Rotate Memory Left with Carry
byte CPU::RLCM(word addr) {
	byte n;
	printf("RLCM: %x", n = getRam(addr));
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x80);
	setRam(addr, (n << 1) | (n >> 7));
	return 16;
}

// Rotate Register Right with Carry
byte CPU::RRC(reg8 reg) {
	byte n;
	printf("RRC: %x", n = get8(reg));
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x01);
	set8(reg, (n >> 1) | (n << 7));
	return 8;
}

// Rotate Memory Right with Carry
byte CPU::RRCM(word addr) {
	byte n;
	printf("RRCM: %x", n = getRam(addr));
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x01);
	setRam(addr, (n >> 1) | (n << 7));
	return 16;
}

// Rotate Register Left through Carry
byte CPU::RL(reg8 reg) {
	byte n; bool c;
	printf("RL: %x %d", n = get8(reg), c = getC());
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x80);
	set8(reg, (n << 1) | c);
	return 8;
}

// Rotate Memory Left through Carry
byte CPU::RLM(word addr) {
	byte n; bool c;
	printf("RLM: %x %d", n = getRam(addr), c = getC());
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x80);
	setRam(addr, (n << 1) | c);
	return 16;
}

// Rotate Register Right through Carry
byte CPU::RR(reg8 reg) {
	byte n; bool c;
	printf("RL: %x %d", n = get8(reg), c = getC());
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x01);
	set8(reg, (n >> 1) | (c << 7));
	return 8;
}

// Rotate Memory Right through Carry
byte CPU::RRM(word addr) {
	byte n; bool c;
	printf("RLM: %x %d", n = getRam(addr), c = getC());
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x01);
	setRam(addr, (n >> 1) | (c << 7));
	return 16;
}

// Shift Register Left (Reset Bit 0)
byte CPU::SLA(reg8 reg) {
	byte n;
	printf("SLA: %x", n = get8(reg));
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x80);
	set8(reg, n << 1);
	return 8;
}

// Shift Memory Left (Reset Bit 0)
byte CPU::SLAM(word addr) {
	byte n;
	printf("SLAM: %x", n = getRam(addr));
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x80);
	setRam(addr, n << 1);
	return 16;
}

// Shift Register Right
byte CPU::SRA(reg8 reg) {
	byte n;
	printf("SRA: %x", n = get8(reg));
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x01);
	set8(reg, (n << 1) | (n & 0x80));
	return 8;
}

// Shift Memory Right
byte CPU::SRAM(word addr) {
	byte n;
	printf("SRAM: %x", n = getRam(addr));
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x01);
	setRam(addr, (n >> 1) | (n & 0x80));
	return 16;
}

// Shift Register Right (Reset Bit 7)
byte CPU::SRL(reg8 reg) {
	byte n;
	printf("SRL: %x", n = get8(reg));
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x01);
	set8(reg, n << 1);
	return 8;
}

// Shift Memory Right (Reset Bit 7)
byte CPU::SRLM(word addr) {
	byte n;
	printf("SRLM: %x", n = getRam(addr));
	setZ(n == 0); setN(0); setH(0);
	setC(n & 0x01);
	setRam(addr, n >> 1);
	return 16;
}

// Swap Hi and Lo Bits of Register
byte CPU::SWAP(reg8 reg) {
	byte n;
	printf("SWAP: %x", n = get8(reg));
	setZ(n == 0); setN(0); setH(0); setC(0);
	set8(reg, (n >> 4) | (n << 4));
	return 8;
}

// Swap Hi and Lo Bits of Memory
byte CPU::SWPM(word addr) {
	byte n;
	printf("SWPM: %x", n = getRam(addr));
	setZ(n == 0); setN(0); setH(0); setC(0);
	setRam(addr, (n >> 4) | (n << 4));
	return 16;
}

// Copy Bit from Register to Z Flag
byte CPU::BIT(byte b, reg8 reg) {
	byte n;
	printf("BIT: %d %x", b, n = get8(reg));
	setN(0); setH(1);
	setZ(!(n & (1 << b)));
	return 8;
}

// Copy Bit from Memory to Z Flag
byte CPU::BITM(byte b, word addr) {
	byte n;
	printf("BIT: %d %x", b, n = getRam(addr));
	setN(0); setH(1);
	setZ(!(n & (1 << b)));
	return 16;
}

// Reset Bit in Register
byte CPU::RES(byte b, reg8 reg) {
	byte n;
	printf("RES: %d %x", b, n = get8(reg));
	set8(reg, n & ~(1 << b));
	return 8;
}

// Reset Bit in Memory
byte CPU::RESM(byte b, word addr) {
	byte n;
	printf("RES: %d %x", b, n = getRam(addr));
	setRam(addr, n & ~(1 << b));
	return 16;
}

// Set Bit in Register
byte CPU::SET(byte b, reg8 reg) {
	byte n;
	printf("RES: %d %x", b, n = get8(reg));
	set8(reg, n | (1 << b));
	return 8;
}

// Set Bit in Memory
byte CPU::SETM(byte b, word addr) {
	byte n;
	printf("RES: %d %x", b, n = getRam(addr));
	setRam(addr, n | (1 << b));
	return 16;
}
// ------------
