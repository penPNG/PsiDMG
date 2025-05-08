#include "dmg.h"

DMG::DMG() {
	cpu = new CPU(ram);
	/*printf("  Standard Instructions\n");
	int c;*/
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

DMG::DMG(const std::string& _rom_path): ram(), cpu(new CPU(ram)) {
	//std::ifstream rom_file(_rom_path);
	FILE* rom_file = fopen(_rom_path.c_str(), "rb");
	if (!rom_file) {
		std::cerr << "Error: Could not open ROM file: " << _rom_path << std::endl;
		return;
	}

	const auto rom_size = std::filesystem::file_size(_rom_path);
	
	std::vector<byte> rom_data(rom_size / sizeof(byte));
	fread(rom_data.data(), rom_size, 1, rom_file);
	//rom_file.read(reinterpret_cast<char*>(rom_data.data()), rom_size);

	if (rom_data.size() > 0x142) {

		ram.rom = rom_data;
	}
}

int DMG::hardwareCycle(int cycles) {
	int oCycles = cpu->runFor(cycles);
	return oCycles;
}

void DMG::loop() {
	static constexpr int cycles = 69920;
	int oCycles = 0;

	using namespace std::chrono;
	auto max_frame_time = 0us;
	auto avg_frame_time = 0us;
	int frames = 0;


	while (!quit) {
		const auto start_time = steady_clock::now();

		int tCycles = (cycles + oCycles);
		oCycles = cpu->runFor(tCycles);

		auto frame_time = duration_cast<microseconds>(steady_clock::now() - start_time);
		max_frame_time = std::max(max_frame_time, frame_time);
		avg_frame_time += frame_time;
		if (++frames == 60) {
			max_frame_time = 0us;
			avg_frame_time = 0us;
			frames = 0;
		}
	}
}