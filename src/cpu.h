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

	bool getZ();
	bool getN();
	bool getH();
	bool getC();

	void setZ(bool);
	void setN(bool);
	void setH(bool);
	void setC(bool);
	// -------------------

	// Ram Functions
	// -------------
	byte getRam(word);
	void setRam(word, byte);

	void push(word);
	word pop();
	// -------------

public:
	Registers registers;
	word PC;	// Program Counter
	bool IME;	// Interrupt Master Enable

private:
	
	//using reg8 = byte;
	/*using reg16 = word;
	static constexpr reg8 A = 1, F = 0, B = 3, C = 2, D = 5, E = 4, H = 7, L = 6;
	static constexpr reg16 AF = 0, BC = 1, DE = 2, HL = 3, SP = 4;*/

	void initInst();

	// Load
	byte LD(reg8, reg8);		// Load standard
	byte LDI(reg8, byte);		// Load Immediate to Register
	byte LDMI(word, byte);		// Load Immediate to Memory
	byte LDRM(reg8, word);		// Load to Register from Memory
	byte LDMR(word, reg8);		// Load to Memory from Register

	byte LD16(reg16, word);		// Load 16 Bit Immediate
	byte LD16SPHL();			// Load to SP from HL
	byte LD16HLSP(sbyte);		// Load to HL from SP + Signed Byte
	byte LD16MSP(word);			// LOAD to Memory from SP
	
	byte POP(reg16);			// POP Stack to 16 Bit Register
	byte PUSH(reg16);			// Push 16 Bit Register to Stack

	// Arithmetic
	byte ADD(reg8);		// ADD Register to A
	byte ADDM(word);	// ADD Memory to A
	byte ADDI(byte);	// ADD Immediate to A

	byte ADC(reg8);		// ADD with Carry Register to A
	byte ADCM(word);	// ADD with Carry emory to A
	byte ADCI(byte);	// ADD with Carry Immediate to A

	byte ADD16(reg16);	// ADD 16 Bit Register to HL
	byte ADD16I(sbyte);	// ADD Signed Byte to SP

	byte SUB(reg8);		// SUB Register from A
	byte SUBM(word);	// SUB Memory from A
	byte SUBI(byte);	// SUB Immediate from A

	byte SBC(reg8);		// SUB with Carry Register from A
	byte SBCM(word);	// SUB with Carry Memory from A
	byte SBCI(byte);	// SUB with Carry Immediate from A

	byte AND(reg8);		// AND Register with A
	byte ANDM(word);	// AND Memory with A
	byte ANDI(byte);	// AND Immediate with A

	byte XOR(reg8);		// XOR Register with A
	byte XORM(word);	// XOR Memory with A
	byte XORI(byte);	// XOR Immediate with A
	
	byte OR(reg8);		// OR Register with A
	byte ORM(word);		// OR Memory with A
	byte ORI(byte);		// OR Immediate with A

	byte CP(reg8);		// Compare Register with A for equality
	byte CPM(word);		// Compare Memory with A for equality
	byte CPI(byte);		// Compare Immediate with A for equality

	byte INC(reg8);		// Increment Register
	byte INCM(word);	// Increment Memory
	byte INC16(reg16);	// Increment 16 Bit Register

	byte DEC(reg8);		// Decrement Register
	byte DECM(word);	// Decrement Memory
	byte DEC16(reg16);	// Decrement 16 Bit Register

	byte CPL();		// One's complement Register A
	byte CCF();		// Flip Carry Flag
	byte DAA();		// Adjust BCD of register A
	byte SCF();		// Set Carry Flag

	// Control
	byte NOP(word);		// Advance PC by one
	byte STOP(word);	// Stop the clock... chaos ensues
	byte HALT(word);	// A special tool we're saving for later
	byte DI(word);		// Disable Interupts
	byte EI(word);		// Enable Interupts

	// Jump / Call
	byte JR(sbyte);			// Jump the value of the Signed Byte
	byte JRS(flag, sbyte);	// Jump the value of the Signed Byte if Flag is set
	byte JRN(flag, sbyte);	// Jump the value of the Signed Byte if Flag is not set

	byte JP(word);			// Jump to given Address
	byte JPHL();			// Jump to HL
	byte JPS(flag, word);	// Jump to given Address if Flag is set
	byte JPN(flag, word);	// Jump to given Address if Flag is not set
	
	byte CALL(word);		// Call given Address
	byte CLLS(flag, word);	// Call given Address if Flag is set
	byte CLLN(flag, word);	// Call given Address if Flag is not set

	byte RST(byte);			// Push PC to Stack, Jump to $0000 + Byte

	byte RET();				// Pop Stack to PC
	byte RTS(flag);			// Pop Stack to PC if Flag is set
	byte RTN(flag);			// Pop Stack to PC if FLag is not set
	byte RETI();		// Return with interupts

	// Bit Manipulation
	byte RLCA();		// Rotate Register A Left with Carry
	byte RLA();			// Rotate Register A Left
	byte RRCA();		// Rotate Register A Right with Carry
	byte RRA();			// Rotate Register A Right
	byte PREF(byte);	// CB Prefix

	// CB Instructions
	byte RLC(reg8);			// Rotate Register Left with Carry
	byte RLCM(word);		// Rotate Memory Left with Carry

	byte RRC(reg8);			// Rotate Register Right with Carry
	byte RRCM(word);		// Rotate Memory Right with Carry

	byte RL(reg8);			// Rotate Register Left
	byte RLM(word);			// Rotate Memory Left

	byte RR(reg8);			// Rotate Register Right
	byte RRM(word);			// Rotate Memory Right

	byte SLA(reg8);			// Shift Register Left (reset bit 0)
	byte SLAM(word);		// Shift Memory Left (reset bit 0)

	byte SRA(reg8);			// Shift Register Right
	byte SRAM(word);		// Shift Memory Right

	byte SWAP(reg8);		// Swap Hi and Lo Bits of Register
	byte SWPM(word);		// Swap Hi and Lo Bits of Memory

	byte SRL(reg8);			// Shift Register Right (reset bit 7)
	byte SRLM(word);		// Shift Memory Right (reset bit 7)

	byte BIT(byte, reg8);	// Copy Bit from Register to Z Flag
	byte BITM(byte, word);	// Copy Bit from Memory to Z Flag

	byte RES(byte, reg8);	// Reset Bit in Register
	byte RESM(byte, word);	// Reset Bit in Memory

	byte SET(byte, reg8);	// Set Bit in Register
	byte SETM(byte, word);	// Set Bit in Memory

private:

	Memory& ram;
	word op;

};