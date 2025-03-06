#pragma once

typedef unsigned char byte;
typedef unsigned short word;

union Register {
	word reg;
	struct {
		byte lo;
		byte hi;
	};
};