#include "cpu.h"

CPU::CPU() {
	regAF.lo = 0x01; regAF.hi = FLAG_Z;
	regBC.reg = 0x0013;
	regDE.reg = 0x00D8;
	regHL.reg = 0x014D;
	regPC.reg = 0x0100;
	regSP.reg = 0xFFFE;
}