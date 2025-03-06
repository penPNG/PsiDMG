#pragma once
#include "ds.h"

// Flags
#define FLAG_Z 0b10000000
#define FLAG_N 0b01000000
#define FLAG_H 0b00100000
#define FLAG_C 0b00010000

class CPU {
public: 
	CPU();

	Register regAF;
	Register regBC;
	Register regDE;
	Register regHL;
	Register regSP;	// Stack pointer
	Register regPC;	// Program counter
};