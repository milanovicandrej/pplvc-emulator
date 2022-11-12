#pragma once
#include <iostream>
#include <iomanip>
#include "Bus.h"

struct STATUS {
	int C = 0; //carry
	int Z = 0; //zero
	int I = 0; //int disable
	int D = 0; //decimal mode
	int B = 0; //break
	int O = 0; //overflow
	int N = 0; //negative
};


struct cpu
{
public:
	using BYTE = unsigned char;
	using WORD = unsigned short;

	bool m_stopFlag;

	//macro parameters
	size_t m_clockFreq;

	Bus* bus;
	
	//registers
	WORD reg_pc;
	BYTE reg_sp;
	STATUS reg_status; 
	BYTE reg_a;
	BYTE reg_x;
	BYTE reg_y;

	WORD abs_addr;
	BYTE current_data;

	cpu(size_t clockFreq) {
		m_clockFreq = clockFreq;
		m_stopFlag = false;
		bus = new Bus();

		abs_addr = 0x00;
		current_data = 0x00;

		this->reset();
	}

	void step();

	void reset() {
		reg_pc = 0xFFFC;
		reg_sp = 0xFF;
		reg_a = 0x00;
		reg_x = 0x00;
		reg_y = 0x00;
	}

	//instructions
	
		//load/store
		void LDA(); void LDX(); void LDY();
		void STA(); void STX(); void STY();

		//register transfers
		void TAX(); void TAY(); void TXA(); void TYA();

		//stack operations
		void TSX(); void TXS(); void PHA(); void PHP(); void PLA(); void PLP();

		//logical
		void AND(); void EOR(); void ORA(); void BIT();

		//arithemetic
		void ADC(); void SBC(); void CMP(); void CPX(); void CPY();

		// inc and dec
		void INC(); void INX(); void INY(); void DEC(); void DEX(); void DEY();

		//shifts
		void ASL(); void LSR(); void ROL(); void ROR();
		
		//jumps and calls
		void JMP() { reg_pc = abs_addr; } void JSR(); void RTS();

		//branches 
		void BCC(); void BCS(); void BEQ(); void BMI(); void BNE(); void BPL(); void BVC(); void BVS();

		//status flag changes
		void CLC(); void CLD(); void CLI(); void CLV(); void SEC(); void SED(); void SEI();

		//system funcs
		void BRK(); void NOP() { return; } void RTI();


	//addressing modes
		//implicit
		//accumulator

		//immediate
		void IMM() {
			BYTE data = bus->fetchByte(reg_pc);
			reg_pc++;
			current_data = data;	
		}

		//zero page
		void ZP() {
			WORD addr = 0x00 + bus->fetchByte(reg_pc);
			reg_pc++;
			BYTE data = bus->fetchByte(addr);
			abs_addr = addr;
			current_data = data;
		}
		
		//zero page x
		void ZPX() {
			WORD addr = 0x00 + bus->fetchByte(reg_pc);
			reg_pc++;
			addr += this->reg_x;
			BYTE data = bus->fetchByte(addr);
			abs_addr = addr;
			current_data = data;
		}

		//zero page y
		void ZPY() {
			WORD addr = 0x00 + bus->fetchByte(reg_pc);
			reg_pc++;
			addr += this->reg_y;
			BYTE data = bus->fetchByte(addr);
			abs_addr = addr;
			current_data = data;
		}

		//absolute
		void ABS() {
			BYTE lo = bus->fetchByte(reg_pc);
			reg_pc++; 
			BYTE hi = bus->fetchByte(reg_pc);
			reg_pc++;

			WORD addr = (hi << 8) | lo;
			abs_addr = addr;
			current_data = bus->fetchByte(addr);
		}

		void ABX() {
			BYTE lo = bus->fetchByte(reg_pc);
			reg_pc++;
			BYTE hi = bus->fetchByte(reg_pc);
			reg_pc++;

			abs_addr = ((hi << 8) | lo) + this->reg_x;
			current_data = bus->fetchByte(abs_addr);
		}

		void ABY() {
			BYTE lo = bus->fetchByte(reg_pc);
			reg_pc++;
			BYTE hi = bus->fetchByte(reg_pc);
			reg_pc++;

			abs_addr = ((hi << 8) | lo) + this->reg_y;
			current_data = bus->fetchByte(abs_addr);
		}

		void IND(){
			BYTE lo = bus->fetchByte(reg_pc);
			reg_pc++;
			BYTE hi = bus->fetchByte(reg_pc);
			reg_pc++;
			WORD addr = (hi<<8) | lo;

			BYTE lo_d = bus->fetchByte(addr);
			BYTE hi_d = bus->fetchByte(addr + 1);

			abs_addr = (hi_d << 8) | lo_d;
			current_data = bus->fetchByte(abs_addr);
		}

		void INDX(){
			WORD addr = 0x00 + bus->fetchByte(reg_pc);
			reg_pc++;
			addr = (addr + reg_x) & 0xFF;
			BYTE lo = bus->fetchByte(addr);
			BYTE hi = bus->fetchByte(addr + 1);
			abs_addr = (hi<<8) | lo;
			current_data = bus->fetchByte(abs_addr);
		}

		void INDY(){
			WORD t = 0x00 + bus->fetchByte(reg_pc);
			reg_pc++;
			abs_addr = (bus->fetchByte(t) << 8) + bus->fetchByte(t + 1) + reg_y;
			current_data = bus->fetchByte(abs_addr);
		}

	~cpu() {
		delete bus;
	}
};

