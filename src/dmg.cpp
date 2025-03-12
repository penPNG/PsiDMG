#include "dmg.h"

DMG::DMG() {
	printf("  Standard Instructions\n");
	for (int i = 0; i < 256; i++) {
		printf("  %X ", i);
		cpu.inst[i](0x3244);
		if ((i+1) % 0x10 == 0) printf("\n\n");
	}
	printf("  CB Instructions\n");
	for (int i = 0; i < 256; i++) {
		printf("  %X ", i);
		cpu.inst[0xCB](i);
		if ((i + 1) % 0x10 == 0) printf("\n\n");
	}
}