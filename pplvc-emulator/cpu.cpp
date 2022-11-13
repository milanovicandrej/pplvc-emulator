#include "cpu.h"

using BYTE = unsigned char;
using WORD = unsigned short;

void cpu::NOP(){ return ;}

void cpu::LDA(){
	reg_a = current_data;
	if(reg_a == 0x00)
		this->reg_status.Z = 1;

	if((reg_a & 0x80) == 0x80)
		this-> reg_status.N = 1; 
}

void cpu::LDX(){
	reg_x = current_data;

	if(reg_x == 0x00)
		this->reg_status.Z = 1;

	if((reg_x & 0x80) == 0x80)
		this-> reg_status.N = 1; 
}

void cpu::LDY(){
	reg_y = current_data;

	if(reg_y == 0x00)
		this->reg_status.Z = 1;

	if((reg_y & 0x80) == 0x80)
		this-> reg_status.N = 1; 
}

void cpu::STA(){
	bus->writeByte(abs_addr,reg_a);
}

void cpu::STX(){
	bus->writeByte(abs_addr,reg_x);
}

void cpu::STY(){
	bus->writeByte(abs_addr,reg_y);
}

void cpu::TAX(){
	current_data = reg_a;
	LDX();
}

void cpu::TAY(){
	current_data = reg_a;
	LDY();
}

void cpu::TXA(){
	current_data = reg_x;
	LDA();
}

void cpu::TYA(){
	current_data = reg_a;
	LDA();
}

void cpu::TSX(){
	current_data = reg_sp;
	LDX();
}

void cpu::TXS(){
	reg_sp = reg_x;
}

void cpu::PHA(){
	bus->writeByte(reg_sp,reg_a);
	if(reg_sp > 0x100)
		reg_sp--;
}

void cpu::PHP(){
	bus->writeByte(reg_sp,reg_status.getSTATUS());
	if(reg_sp > 0x100)
		reg_sp--;
}

void cpu::PLA(){
	reg_a = bus->fetchByte(reg_sp);
	if(reg_sp < 0x1FF)
		reg_sp++;
}

void cpu::PLP(){
	reg_status.setSTATUS(bus->fetchByte(reg_sp));
	if(reg_sp < 0x1FF)
		reg_sp++;
}

void cpu::AND(){
	current_data = reg_a & current_data;
	LDA();
}

void cpu::EOR(){
	current_data = reg_a ^ current_data;
	LDA();
}

void cpu::ORA(){
	current_data = reg_a | current_data;
	LDA();
}

void cpu::BIT(){
	BYTE t = reg_a;
	t = t & current_data;

	if(t == 0x00)
		reg_status.Z = 1;
	
	reg_status.V = (current_data & 0b01000000) >> 6;
	reg_status.N = (current_data & 0b10000000) >> 7;
}

void cpu::ADC(){
	
	WORD temp = reg_a + current_data + reg_status.C;
	//1 0 0 0 0 0 0 0 0 ->0x100

	reg_status.C = temp > 0xFF;
	
	reg_status.N = reg_a & 0b10000000 == 0b10000000;
	reg_status.V = !((reg_a ^ current_data) & 0x80) && ((reg_a ^ temp) & 0x80);

	reg_a = reg_a + current_data + reg_status.C;
	reg_status.Z = reg_a == 0x00;
}

void cpu::SBC(){
	WORD temp = reg_a - current_data - (1- reg_status.C);

	reg_status.C = current_data + !reg_status.C > reg_a;
	reg_status.N = reg_a & 0b10000000 == 0b10000000;
	reg_status.V = !((reg_a ^ current_data) & 0x80) && ((reg_a ^ temp) & 0x80);
	
	reg_a = reg_a - current_data - (1- reg_status.C);
	reg_status.Z = reg_a == 0x00;
}

void cpu::CMP(){
	reg_status.C = reg_a >= current_data;
	reg_status.Z = reg_a == current_data;
	reg_status.N = ((reg_a - current_data) & 0b10000000) >> 7;
}

void cpu::CPX(){
	reg_status.C = reg_x >= current_data;
	reg_status.Z = reg_x == current_data;
	reg_status.N = ((reg_x - current_data) & 0b10000000) >> 7;
}

void cpu::CPY(){
	reg_status.C = reg_y >= current_data;
	reg_status.Z = reg_y == current_data;
	reg_status.N = ((reg_y - current_data) & 0b10000000) >> 7;
}

void cpu::INC(){
	current_data++;
	bus->writeByte(abs_addr,current_data);

	reg_status.Z = current_data == 0;
	reg_status.N = (current_data & 0b10000000) >> 7;
}

void cpu::INX(){
	reg_x++;
	reg_status.Z = reg_x == 0;
	reg_status.N = (reg_x & 0b10000000) >> 7;
}

void cpu::INY(){
	reg_y++;
	reg_status.Z = reg_y == 0;
	reg_status.N = (reg_y & 0b10000000) >> 7;
}

void cpu::DEC(){
	current_data--;
	bus->writeByte(abs_addr,current_data);

	reg_status.Z = current_data == 0;
	reg_status.N = (current_data & 0b10000000) >> 7;
}

void cpu::DEX(){
	reg_x--;
	reg_status.Z = reg_x == 0;
	reg_status.N = (reg_x & 0b10000000) >> 7;
}

void cpu::DEY(){
	reg_y--;
	reg_status.Z = reg_y == 0;
	reg_status.N = (reg_y & 0b10000000) >> 7;
}

void cpu::ASL(bool isMem){
	if(isMem){
		reg_status.C = (current_data & 0b10000000) >> 7;
		current_data << 1;
		bus->writeByte(abs_addr,current_data);
		reg_status.Z = current_data == 0;
		reg_status.N = (current_data & 0b10000000) >> 7;
	}else{
		reg_status.C = (reg_a & 0b10000000) >> 7;
		reg_a << 1;
		reg_status.Z = reg_a == 0;
		reg_status.N = (reg_a & 0b10000000) >> 7;
	}
}

void cpu::LSR(bool isMem){
	if(isMem){
		reg_status.C = current_data & 0b00000001;
		current_data = current_data / 2;
		bus->writeByte(abs_addr,current_data);
		reg_status.Z = current_data == 0;
		reg_status.N = (current_data & 0b10000000) >> 7;
	}else{
		reg_status.C = reg_a & 0b10000001;
		reg_a = reg_a / 2;
		reg_status.Z = reg_a == 0;
		reg_status.N = (reg_a & 0b10000000) >> 7;
	}
}

void cpu::ROL(bool isMem){
	if(isMem){
		BYTE oldseven = (current_data & 0b10000000) >> 7;

		current_data = (current_data << 1) | reg_status.C;
		bus->writeByte(abs_addr,current_data);
		reg_status.C = oldseven;
		
		reg_status.Z = current_data == 0;
		reg_status.N = (current_data & 0b10000000) >> 7;
	}else{
		BYTE oldseven = (reg_a & 0b10000000) >> 7;

		reg_a = (reg_a << 1) | reg_status.C;
		reg_status.C = oldseven;
		reg_status.Z = reg_a == 0;
		reg_status.N = (reg_a & 0b10000000) >> 7;
	}	
}

void cpu::ROR(bool isMem){
	if(isMem){

		BYTE oldzero = (current_data & 0b00000001);
		
		current_data = (current_data >> 1) | (reg_status.C << 7);
		bus->writeByte(abs_addr,current_data);

		reg_status.C = oldzero;

		reg_status.Z = current_data == 0;
		reg_status.N = (reg_a & 0b10000000) >> 7;
	}else{

		BYTE oldzero = (reg_a & 0b00000001);
		
		reg_a = (reg_a >> 1) | (reg_status.C << 7);
		reg_status.C = oldzero;

		
		reg_status.Z = reg_a == 0;
		reg_status.N = (reg_a & 0b10000000) >> 7;
	}
}

void cpu::JMP(){ reg_pc = abs_addr; }

void cpu::JSR(){

	PHP();

	BYTE lo = reg_pc & 0x00FF;
	BYTE hi = reg_pc >> 8 & 0x00FF;

	bus->writeByte(reg_sp,hi);
	if(reg_sp > 0x100)
		reg_sp--;
	
	
	bus->writeByte(reg_sp,lo);
	if(reg_sp > 0x100)
		reg_sp--;

	reg_pc = abs_addr;
}

void cpu::RTS(){

	BYTE lo = bus->fetchByte(reg_sp);
	if(reg_sp < 0x1FF)
		reg_sp++;

	BYTE hi = bus->fetchByte(reg_sp);
	if(reg_sp < 0x1FF)
		reg_sp++;

	PLP();

	reg_pc = (hi << 8)|lo;
}



//execute one instruction
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
	else if (ins == 0xAD){
		//LDA - ABS
		cycleCount = 4;
		ABS();
		LDA();
	}
	else if (ins == 0x4C) {
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