#pragma once
#include "ds.h"
#include <stdio.h>

// Flags
#define FLAG_Z 0b10000000
#define FLAG_N 0b01000000
#define FLAG_H 0b00100000
#define FLAG_C 0b00010000

struct Registers {
	Register regAF;
	Register regBC;
	Register regDE;
	Register regHL;
	Register regSP;	// Stack pointer
	Register regPC;	// Program counter
};

class CPU {
public:
	CPU();

	inline static void (*inst[256]) (word);
	inline static void (*CBInst[256]) (word);

	// Regiseter Functions
	// -------------------
	byte getA();
	byte getF();
	word getAF();
	byte getB();
	byte getC();
	word getBC();
	byte getD();
	byte getE();
	word getDE();
	byte getH();
	byte getL();
	word getHL();
	word getSP();
	word getPC();

	void setA(byte);
	void setF(byte);
	void setAF(word);
	void setAF(byte, byte);
	void setB(byte);
	void setC(byte);
	void setBC(word);
	void setBC(byte, byte);
	void setD(byte);
	void setE(byte);
	void setDE(word);
	void setDE(byte, byte);
	void setH(byte);
	void setL(byte);
	void setHL(word);
	void setHL(byte, byte);
	void setSP(word);
	void setPC(word);
	void setPCLo(byte);
	void setPCHi(byte);
	// -------------------

private:
	void initInst();

	// Load
	static void LD(word);
	static void LDH(word);
	static void LD16(word);
	static void POP(word);
	static void PUSH(word);

	// Arithmetic
	static void ADD(word);		// ADD Operation
	static void ADC(word);		// ADD with Carry
	static void ADD16(word);	// 16 bit ADD Operation
	static void SUB(word);		// SUB Operation
	static void SBC(word);		// SUB with Carry
	static void AND(word);		// AND Operation
	static void XOR(word);		// ... TODO FINISH NOTES
	static void OR(word);
	static void CP(word);
	static void INC(word);
	static void INC16(word);	// 16 bit INC Operation
	static void DEC(word);
	static void DEC16(word);	// 16 bit DEC operation
	static void CPL(word);		// One's complement register A
	static void CCF(word);		// Flip the carry flag CY
	static void DAA(word);		// Adjust BCD of register A
	static void SCF(word);		// Set the carry flag CY

	// Control
	static void NOP(word);		// Advance PC by one
	static void STOP(word);		// Stop the clock... chaos ensues
	static void HALT(word);
	static void DI(word);		// Disable Interupts
	static void EI(word);		// Enable Interupts

	// Jump / Call
	static void JR(word);		// Jump Relative
	static void JP(word);		// Jump Absolute
	static void CALL(word);		// Call
	static void RST(word);		// Reset?
	static void RET(word);		// Return from call
	static void RETI(word);		// Return with interupts

	// Bit Manipulation
	static void RLCA(word);
	static void RLA(word);
	static void RRCA(word);
	static void RRA(word);
	static void PREF(word);

	// CB Instructions
	static void RLC(word);
	static void RRC(word);
	static void RL(word);
	static void RR(word);
	static void SLA(word);
	static void SRA(word);
	static void SWAP(word);
	static void SRL(word);
	static void BIT(word);
	static void RES(word);
	static void SET(word);

	static void no(word);

private:
	Registers registers;
	word opc;

};