#include "cpu.h"

CPU::CPU() {

	registers.regAF.lo = 0x01; registers.regAF.hi = FLAG_Z;
	registers.regBC.reg = 0x0013;
	registers.regDE.reg = 0x00D8;
	registers.regHL.reg = 0x014D;
	registers.regPC.reg = 0x0100;
	registers.regSP.reg = 0xFFFE;
}

// Reigster Functions
// ------------------
byte CPU::getA() { return registers.regAF.lo; }
byte CPU::getF() { return registers.regAF.hi; }
word CPU::getAF() {return registers.regAF.reg; }
byte CPU::getB() { return registers.regBC.lo; }
byte CPU::getC() { return registers.regBC.hi; }
word CPU::getBC() { return registers.regBC.reg; }
byte CPU::getD() { return registers.regDE.lo; }
byte CPU::getE() { return registers.regDE.hi; }
word CPU::getDE() { return registers.regDE.reg; }
byte CPU::getH() { return registers.regHL.lo; }
byte CPU::getL() { return registers.regHL.hi; }
word CPU::getHL() { return registers.regHL.reg; }
word CPU::getSP() { return registers.regSP.reg; }
word CPU::getPC() { return registers.regPC.reg; }

void CPU::setA(byte d) { registers.regAF.lo = d; }
void CPU::setF(byte d) { d &= 0xF0; registers.regAF.hi &= d; } // Set Flags
void CPU::setAF(word d) { d &= 0xFFF0; registers.regAF.reg = d; }
void CPU::setAF(byte l, byte h) { h &= 0xF0; registers.regAF.reg = (h & (l<<8)); }
void CPU::setB(byte d) { registers.regBC.lo = d; }
void CPU::setC(byte d) { registers.regBC.hi = d; }
void CPU::setBC(word d) { registers.regBC.reg = d; }
void CPU::setBC(byte l, byte h) { registers.regBC.reg = (h & (l<<8)); }
void CPU::setD(byte d) { registers.regDE.lo = d; }
void CPU::setE(byte d) { registers.regDE.hi = d; }
void CPU::setDE(word d) { registers.regDE.reg = d; }
void CPU::setDE(byte l, byte h) { registers.regDE.reg = (h & (l<<8)); }
void CPU::setH(byte d) { registers.regHL.lo = d; }
void CPU::setL(byte d) { registers.regHL.hi = d; }
void CPU::setHL(word d) { registers.regHL.reg = d; }
void CPU::setHL(byte l, byte h) { registers.regHL.reg = (h & (l<<8)); }
void CPU::setSP(word d) { registers.regSP.reg = d; }
void CPU::setPC(word d) { registers.regPC.reg = d; }	// Set Program Counter
void CPU::setPCLo(byte d) { registers.regPC.lo = d; }	// Set Lo byte of PC
void CPU::setPCHi(byte d) { registers.regPC.hi = d; }	// Set Hi byte of PC
// -------------------