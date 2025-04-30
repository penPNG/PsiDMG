#include "dmg.h"

DMG::DMG() {
	cpu = new CPU(ram);
	printf("  Standard Instructions\n");
	int c;
	/*for (int i = 0; i < 256; i++) {
		printf("  $%X ", i);
		c = cpu->exec(i);
		printf(" #%d", c);
		if ((i+1) % 0x10 == 0) printf("\n\n");
	}*/
	//printf("  CB Instructions\n");
	//for (int i = 0; i < 256; i++) { ram.ram[0x100 + i] = i; }
	//cpu->PC = 0x100;
	//for (int i = 0; i < 256; i++) {
	//	printf("  $%X ", i);
	//	c = cpu->exec(0xCB);
	//	//printf(" #%d", c);
	//	if ((i + 1) % 0x10 == 0) printf("\n\n");
	//}
	/*cpu.inst[0x06](0x0625);
	cpu.inst[0x40](0x4000);*/
	//cpu->exec(0x3E);
	//cpu->exec(0x40);
	//printf("\nResult: %X", cpu->get8(A));
}

DMG::DMG(const std::string &rom_path) {
	std::ifstream rom_file(rom_path);
	if (!rom_file) {
		std::cerr << "Error: Could not open ROM file: " << rom_path << std::endl;
		return;
	}

	const auto rom_size = std::filesystem::file_size(rom_path);
	
	std::vector<byte> rom_data(rom_size / sizeof(byte));
	rom_file.read(reinterpret_cast<char*>(rom_data.data()), rom_size);
	rom = rom_data;

	cpu = new CPU(ram);
}

void DMG::loop() {
	static constexpr int cycles = 69920;
	int oCycles = 0;
	int frame = 0;

	while (!quit) {
		//cpu->exec(ram.getRam(cpu->PC++));
	}
}