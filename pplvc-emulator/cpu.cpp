#include "cpu.h"

using BYTE = unsigned char;
using WORD = unsigned short;

void cpu::step() {
	size_t cycleCount = 0;
	BYTE ins = bus->fetchByte(reg_pc);
	reg_pc++;

	if (ins == 0xA9) {
		//LDA - Imm
		cycleCount = 2;
		IMM();
		LDA();
	}
	else if (ins == 0xA5) {
		//LDA - ZP
		cycleCount = 3;
		ZP();
		LDA();
	}
	else if (ins == 0xB5) {
		//LDA - ZPX
		cycleCount = 4;
		ZPX();
		LDA();
	}
	else if (ins == 0xAD) {
		//JMP - ABSOLUTE
		ABS();
		JMP();
	}
	else {
		NOP();
		cycleCount = 1;
	}
	
	//waiting mechanism to be in time with clock
}