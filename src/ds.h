#pragma once

typedef unsigned char byte;
typedef unsigned short word;

union Register {
	word reg;	// Combined Registers
	struct {
		byte lo;	// First Byte (B)
		byte hi;	// Last Byte (C)
	};
};