#pragma once
#include "../memory/memory.h"
#include "../cpu/cpu.h"
#include "ds.h"
#include <stdio.h>

class DMG{
public:
	DMG();

	void loop();

	CPU* cpu;
	Memory ram;

private:
	bool quit = false;
};