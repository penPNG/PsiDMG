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

	Registers registers;

};