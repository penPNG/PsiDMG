#pragma once

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short word;
typedef signed short sword;

union Registers {
	byte reg8[10];
	word reg16[5];
};

enum reg8 {
	F,
	A,
	C,
	B,
	E,
	D,
	L,
	H
};

enum reg16 {
	AF,
	BC,
	DE,
	HL,
	SP
};

enum flag {
	flagZ = 0b10000000,	// Set if an operation equals 0
	flagN = 0b01000000,	// Set if previous instruction has been as subtraction
	flagH = 0b00100000,	// Set for Carry of lower 4 bits
	flagC = 0b00010000	// Set if 1 Carried
};