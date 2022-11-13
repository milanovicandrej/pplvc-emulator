#pragma once
#include <iostream>
#include <map>
#include <iomanip>
#include "Bus.h"


using BYTE = unsigned char;
using WORD = unsigned short;

struct STATUS {
	BYTE C = 0; //carry
	BYTE Z = 0; //zero
	BYTE I = 0; //disable
	BYTE D = 0; //decimal mode
	BYTE V = 0; //overflow
	BYTE N = 0; //negative

	BYTE getSTATUS(){
		BYTE temp = 0x00
			| C | (Z<<1) | (I<<2) | (D<<3) | (V<<6) | (N<<7)
		;

		return temp;
	}

	void setSTATUS(BYTE status){
		C = status & 0b00000001;
		Z = status & 0b00000010;
		I = status & 0b00000100;
		D = status & 0b00001000;
		V = status & 0b01000000;
		N = status & 0b10000000;
	}
};


struct cpu
{
public:
	using BYTE = unsigned char;
	using WORD = unsigned short;

	
	struct Instruction{
		int cycleCount = 2;
		void (cpu::*addMode)() = nullptr;
		void (cpu::*operation)() = nullptr;
		std::string mneu = "";
		BYTE opcode = 0x00;
	};


	bool m_stopFlag;

	//macro parameters
	size_t m_clockFreq;

	Bus* bus;
	
	std::map<BYTE,Instruction> m_instructionSet;

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
	void initInsSet();
	void reset();

	//instructions
	
		//load/store
		void LDA(); void LDX(); void LDY(); //done

		void STA(); void STX(); void STY(); //done

		//register transfers
		void TAX(); void TAY(); void TXA(); void TYA(); //done

		//stack operations
		void TSX(); void TXS(); void PHA(); void PHP(); void PLA(); void PLP(); //done

		//logical
		void AND(); void EOR(); void ORA(); void BIT(); //done

		//arithemetic
		void ADC(); void SBC(); void CMP(); void CPX(); void CPY(); //done

		// inc and dec
		void INC(); void INX(); void INY(); void DEC(); void DEX(); void DEY(); //done 

		//shifts
		void ASL(bool isMem); void LSR(bool isMem); void ROL(bool isMem); void ROR(bool isMem); // done
		
		//jumps and calls
		void JMP(); void JSR(); void RTS(); //done

		//branches 
		void BCC(); void BCS(); void BEQ(); void BMI(); void BNE(); void BPL(); void BVC(); void BVS();

		//status flag changes
		void CLC(); void CLD(); void CLI(); void CLV(); void SEC(); void SED(); void SEI();

		//system funcs
		void BRK(); void NOP(); void RTI();


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

