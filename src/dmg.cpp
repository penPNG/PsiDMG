#include "dmg.h"

DMG::DMG() {
	cpu;
	memory.memory[32] = 124;
	printf("%x", memory.memory[32]);
}