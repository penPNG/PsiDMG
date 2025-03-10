#pragma once
#include "ds.h"

// Flags
#define FLAG_Z 0b10000000
#define FLAG_N 0b01000000
#define FLAG_H 0b00100000
#define FLAG_C 0b00010000

struct Registers {
	Register regAF;
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

	void (CPU::*inst[256]) (word);

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
	constexpr void initInst();

	// Load
	void LD(word);
	void LD16(word);
	void POP(word);
	void PUSH(word);

	// Arithmetic
	void ADD(word);		// ADD Operation
	void ADC(word);		// ADD with Carry
	void ADD16(word);	// 16 bit ADD Operation
	void SUB(word);		// SUB Operation
	void SBC(word);		// SUB with Carry
	void AND(word);		// AND Operation
	void XOR(word);		// ... TODO FINISH NOTES
	void OR(word);
	void CP(word);
	void INC(word);
	void INC16(word);	// 16 bit INC Operation
	void DEC(word);
	void DEC16(word);	// 16 bit DEC operation
	void CPL(word);		// One's complement register A
	void CCF(word);		// Flip the carry flag CY
	void DAA(word);		// Adjust BCD of register A
	void SCF(word);		// Set the carry flag CY

	// Control
	void NOP(word);		// Advance PC by one
	void STOP(word);	// Stop the clock... chaos ensues
	void HALT(word);
	void PREF(word);	// See CB Instructions
	void DI(word);		// Disable Interupts
	void EI(word);		// Enable Interupts

	// Jump / Call
	void JR(word);		// Jump Relative
	void JP(word);		// Jump Absolute
	void CALL(word);	// Call
	void RST(word);		// Reset?
	void RET(word);		// Return from call
	void RETI(word);	// Return with interupts

private:
	Registers registers;
	word opc;

};