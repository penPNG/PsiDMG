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
	H,
	P,
	S
};

enum class PPUState {
	HBLANK,
	VBLANK,
	OAM,
	VRAM,
	NONE
};

enum class MBC {
	NONE,
	MBC1,
	MBC2,
	MBC3,
	MBC5
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

enum HWRegisters {
	P1 = 0x00,		// Joypad
	JOYPAD = 0x00,	// Joypad
	SB = 0x01,		// Serial Transfer Data
	SC = 0x02,		// Serial Transfer Control
	DIV = 0x04,		// Divider Register
	TIMA = 0x05,	// Timer Counter
	TMA = 0x06,		// Timer Modulo
	TAC = 0x07,		// Timer Control
	IF = 0x0F,		// Interupt Flag
	NR10 = 0x10,	// Sound Channel 1 Sweep
	NR11 = 0x11,	// Sound Channel 1 Sound Length Timer & Duty Cycle
	NR12 = 0x12,	// Sound Channel 1 Volume & Envelope
	NR13 = 0x13,	// Sound Channel 1 Period Lo
	NR14 = 0x14,	// Sound Channel 1 Period Hi & Control
	NR21 = 0x16,	// Sound Channel 2 Sound Length Timer & Duty Cycle
	NR22 = 0x17,	// Sound Channel 2 Volume & Envelope
	NR23 = 0x18,	// Sound Channel 2 Period Lo
	NR24 = 0x19,	// Sound Channel 2 Period Hi & Control
	NR30 = 0x1A,	// Sound Channel 3 DAC Enable
	NR31 = 0x1B,	// Sound Channel 3 Length Timer
	NR32 = 0x1C,	// Sound Channel 3 Output Level
	NR33 = 0x1D,	// Sound Channel 3 Period Lo
	NR34 = 0x1E,	// Sound Channel 3 Period Hi & Control
	NR41 = 0x20,	// Sound Channel 4 Length Timer
	NR42 = 0x21,	// Sound Channel 4 Volume & Envelope
	NR43 = 0x22,	// Sound Channel 4 Frequency & Randomness
	NR44 = 0x23,	// Sound Channel 4 Control
	NR50 = 0x24,	// Master Volume & VIN Panning
	NR51 = 0x25,	// Sound Panning
	NR52 = 0x26,	// Sound On/O
	LCDC = 0x40,	// LCD Control
	STAT = 0x41,	// LCD Status
	SCY = 0x42,		// Viewport Y Position
	SCX = 0x43,		// Viewport X Position
	LY = 0x44,		// LCDC Y Coordinate
	LYC = 0x45,		// LY Compare
	DMA = 0x46,		// OAM DMA Source Address & Start
	BGP = 0x47,		// Background Palette Data
	OBP0 = 0x48,	// Object Palette 0 Data
	OBP1 = 0x49,	// Object Palette 1 Data
	WY = 0x4A,		// Window Y Position
	WX = 0x4B,		// Window X Position
	IE = 0xFF		// Interupt Enable
};