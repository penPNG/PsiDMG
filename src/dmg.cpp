#include "dmg.h"

DMG::DMG() {
	cpu = new CPU(ram);
	printf("  Standard Instructions\n");
	for (int i = 0; i < 256; i++) {
		printf("  %X ", i);
		cpu->exec(i);
		if ((i+1) % 0x10 == 0) printf("\n\n");
	}
	printf("  CB Instructions\n");
	for (int i = 0; i < 256; i++) {
		printf("  %X ", i);
		cpu->exec(0xCB);
		if ((i + 1) % 0x10 == 0) printf("\n\n");
	}
	/*cpu.inst[0x06](0x0625);
	cpu.inst[0x40](0x4000);*/
	printf("\nResult: %X", cpu->get16(AF));
}