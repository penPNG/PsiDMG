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
#include "context.h"

class DMG{
public:
	DMG();
	DMG(const std::string &rom_path, Context& context);

	void loop();

	//void loadRom(const char* rom);

	CPU* cpu;
	Context context;
	Memory ram;

	std::vector<byte> rom;

private:
	bool quit = false;
};