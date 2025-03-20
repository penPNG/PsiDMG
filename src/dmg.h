#pragma once
#include "memory.h"
#include "cpu.h"
#include "ds.h"
#include <stdio.h>

class DMG{
public:
	DMG();

	CPU* cpu;
	Memory ram;
};