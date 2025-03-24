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
	P1 = 0xFF00,	// Joypad
	JOYPAD = 0xFF00,// Joypad
	SB = 0xFF01,	// Serial Transfer Data
	SC = 0xFF02,	// Serial Transfer Control
	DIV = 0xFF04,	// Divider Register
	TIMA = 0xFF05,	// Timer Counter
	TMA = 0xFF06,	// Timer Modulo
	TAC = 0xFF07,	// Timer Control
	IF = 0xFF0F,	// Interupt Flag
	NR10 = 0xFF10,	// Sound Channel 1 Sweep
	NR11 = 0xFF11,	// Sound Channel 1 Sound Length Timer & Duty Cycle
	NR12 = 0xFF12,	// Sound Channel 1 Volume & Envelope
	NR13 = 0xFF13,	// Sound Channel 1 Period Lo
	NR14 = 0xFF14,	// Sound Channel 1 Period Hi & Control
	NR21 = 0xFF16,	// Sound Channel 2 Sound Length Timer & Duty Cycle
	NR22 = 0xFF17,	// Sound Channel 2 Volume & Envelope
	NR23 = 0xFF18,	// Sound Channel 2 Period Lo
	NR24 = 0xFF19,	// Sound Channel 2 Period Hi & Control
	NR30 = 0xFF1A,	// Sound Channel 3 DAC Enable
	NR31 = 0xFF1B,	// Sound Channel 3 Length Timer
	NR32 = 0xFF1C,	// Sound Channel 3 Output Level
	NR33 = 0xFF1D,	// Sound Channel 3 Period Lo
	NR34 = 0xFF1E,	// Sound Channel 3 Period Hi & Control
	NR41 = 0xFF20,	// Sound Channel 4 Length Timer
	NR42 = 0xFF21,	// Sound Channel 4 Volume & Envelope
	NR43 = 0xFF22,	// Sound Channel 4 Frequency & Randomness
	NR44 = 0xFF23,	// Sound Channel 4 Control
	NR50 = 0xFF24,	// Master Volume & VIN Panning
	NR51 = 0xFF25,	// Sound Panning
	NR52 = 0xFF26,	// Sound On/Off
	LCDC = 0xFF40,	// LCD Control
	STAT = 0xFF41,	// LCD Status
	SCY = 0xFF42,	// Viewport Y Position
	SCX = 0xFF43,	// Viewport X Position
	LY = 0xFF44,	// LCDC Y Coordinate
	LYC = 0xFF45,	// LY Compare
	DMA = 0xFF46,   // OAM DMA Source Address & Start
	BGP = 0xFF47,	// Background Palette Data
	OBP0 = 0xFF48,	// Object Palette 0 Data
	OBP1 = 0xFF49,	// Object Palette 1 Data
	WY = 0xFF4A,	// Window Y Position
	WX = 0xFF4B,	// Window X Position
	IE = 0xFFFF		// Interupt Enable
};