#pragma once
#include "ds.h"
#include <stdio.h>
#include "memory.h"
#include <cstddef>

// Flags
#define FLAG_Z 0b10000000
#define FLAG_N 0b01000000
#define FLAG_H 0b00100000
#define FLAG_C 0b00010000

class CPU {
public:
	CPU(Memory&);

	void exec(word);

	//inline static void (*inst[256]) (word);
	/*void (CPU::*CBInst[256]) (word);
	inline static byte (*getReg[8]) ();
	inline static word (*getReg16[6]) ();
	inline static void (*setReg[8]) (byte);
	inline static void (*setReg16[6]) (word);*/

	// Regiseter Functions
	// -------------------
	byte get8(reg8);
	word get16(reg16);

	void set8(reg8, byte);
	void set16(reg16, word);
	void set16(reg16, byte, byte);

	void setZ(bool);
	void setN(bool);
	void setH(bool);
	void setC(bool);
	// -------------------

	byte getRam(word);
	void setRam(word, byte);

private:
	Registers registers;
	word PC;
	
	//using reg8 = byte;
	/*using reg16 = word;
	static constexpr reg8 A = 1, F = 0, B = 3, C = 2, D = 5, E = 4, H = 7, L = 6;
	static constexpr reg16 AF = 0, BC = 1, DE = 2, HL = 3, SP = 4;*/

	void initInst();

	// Load
	void LD(reg8, reg8);		// Load standard
	void LDI(reg8, byte);		// Load Immediate
	void LDRM(reg8, word);		// Load to Register from Memory
	void LDMR(word, reg8);		// Load to Memory from Register

	void LD16(reg16, word);		// Load 16 Bit Immediate
	void LD16SPHL();			// Load to SP from HL
	void LD16HLSP(sbyte);		// Load to HL from SP + Signed Byte
	void LD16MSP(word);			// LOAD to Memory from SP
	
	void POP(reg16);			// POP Stack to 16 Bit Register
	void PUSH(reg16);			// Push 16 Bit Register to Stack

	// Arithmetic
	void ADD(reg8);		// ADD Register to A
	void ADDM(word);	// ADD Memory to A
	void ADDI(byte);	// ADD Immediate to A

	void ADC(reg8);		// ADD with Carry Register to A
	void ADCM(word);	// ADD with Carry emory to A
	void ADCI(byte);	// ADD with Carry Immediate to A

	void ADD16(reg16);	// ADD 16 Bit Register to HL
	void ADD16I(sbyte);	// ADD Signed Byte to SP

	void SUB(reg8);		// SUB Register from A
	void SUBM(word);	// SUB Memory from A
	void SUBI(byte);	// SUB Immediate from A

	void SBC(reg8);		// SUB with Carry Register from A
	void SBCM(word);	// SUB with Carry Memory from A
	void SBCI(byte);	// SUB with Carry Immediate from A

	void AND(reg8);		// AND Register with A
	void ANDM(word);	// AND Memory with A
	void ANDI(byte);	// AND Immediate with A

	void XOR(reg8);		// XOR Register with A
	void XORM(word);	// XOR Memory with A
	void XORI(byte);	// XOR Immediate with A

	void OR(reg8);		// OR Register with A
	void ORM(word);		// OR Memory with A
	void ORI(byte);		// OR Immediate with A

	void CP(reg8);		// Compare Register with A for equality
	void CPM(word);		// Compare Memory with A for equality
	void CPI(byte);		// Compare Immediate with A for equality

	void INC(reg8);		// Increment Register
	void INCM(word);	// Increment Memory
	void INC16(reg16);	// Increment 16 Bit Register

	void DEC(reg8);		// Decrement Register
	void DECM(word);	// Decrement Memory
	void DEC16(reg16);	// Decrement 16 Bit Register

	void CPL(word);		// One's complement Register A
	void CCF(word);		// Flip Carry Flag
	void DAA(word);		// Adjust BCD of register A
	void SCF(word);		// Set Carry Flag

	// Control
	void NOP(word);		// Advance PC by one
	void STOP(word);	// Stop the clock... chaos ensues
	void HALT(word);	// A special tool we're saving for later
	void DI(word);		// Disable Interupts
	void EI(word);		// Enable Interupts

	// Jump / Call
	void JR(sbyte);			// Jump the value of the Signed Byte
	void JRS(flag, sbyte);	// Jump the value of the Signed Byte if Flag is set
	void JRN(flag, sbyte);	// Jump the value of the Signed Byte if Flag is not set

	void JP(word);			// Jump to given Address
	void JPHL();			// Jump to HL
	void JPS(flag, word);	// Jump to given Address if Flag is set
	void JPN(flag, word);	// Jump to given Address if Flag is not set

	void CALL(word);		// Call given Address
	void CLLS(flag, word);	// Call given Address if Flag is set
	void CLLN(flag, word);	// Call given Address if Flag is not set

	void RST(byte);			// Push PC to Stack, Jump to $0000 + Byte

	void RET();				// Pop stack to PC
	void RTS(flag);			// Pop stack to PC if Flag is set
	void RTN(flag);			// Pop stack to PC if FLag is not set
	void RETI(word);		// Return with interupts

	// Bit Manipulation
	void RLCA();		// Rotate Register A Left with Carry
	void RLA();			// Rotate Register A Left
	void RRCA();		// Rotate Register A Right with Carry
	void RRA();			// Rotate Register A Right
	void PREF(byte);	// CB Prefix

	// CB Instructions
	void RLC(reg8);			// Rotate Register Left with Carry
	void RLCM(word);		// Rotate Memory Left with Carry
	void RRC(reg8);			// Rotate Register Right with Carry
	void RRCM(word);		// Rotate Memory Right with Carry

	void RL(reg8);			// Rotate Register Left
	void RLM(word);			// Rotate Memory Left

	void RR(reg8);			// Rotate Register Right
	void RRM(word);			// Rotate Memory Right

	void SLA(reg8);			// Shift Register Left (reset bit 0)
	void SLAM(word);		// Shift Memory Left (reset bit 0)

	void SRA(reg8);			// Shift Register Right
	void SRAM(word);		// Shift Memory Right

	void SWAP(reg8);		// Swap Hi and Lo Bits of Register
	void SWPM(word);		// Swap Hi and Lo Bits of Memory

	void SRL(reg8);			// Shift Register Right (reset bit 0)
	void SRLM(word);		// Shift Memory Right (reset bit 0)

	void BIT(byte, reg8);	// Copy Bit from Register to Z Flag
	void BITM(byte, word);	// Copy Bit from Memory to Z Flag

	void RES(byte, reg8);	// Reset Bit in Register
	void RES(byte, word);	// Reset Bit in Memory

	void SET(byte, reg8);	// Set Bit in Register
	void SETM(byte, word);	// Set Bit in Memory

	void no(word);

private:

	Memory& ram;
	word opc;

};