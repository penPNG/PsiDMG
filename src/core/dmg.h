#pragma once
#include "../memory/memory.h"
#include "../cpu/cpu.h"
#include "ds.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <string>

class DMG{
public:
	DMG();
	DMG(const std::string &rom_path);

	void loop();

	int hardwareCycle(int cycles);

	//void loadRom(const char* rom);

	CPU* cpu;
	Memory ram;


private:
	bool quit = false;
};