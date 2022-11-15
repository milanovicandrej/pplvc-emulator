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
	bus->writeByte(0x0100 + reg_sp,reg_a);
	if(reg_sp > 0x00)
		reg_sp--;
}

void cpu::PHP(){
	BYTE stat = reg_status.getSTATUS();
	stat = stat | 0b00110000;
	bus->writeByte(0x0100 + reg_sp,stat);
	if(reg_sp > 0x00)
		reg_sp--;
}

void cpu::PLA(){
	reg_a = bus->fetchByte(0x100 + reg_sp);
	if(reg_sp < 0xFF)
		reg_sp++;
}

void cpu::PLP(){
	reg_status.setSTATUS(bus->fetchByte(0x100 + reg_sp));
	if(reg_sp < 0xFF)
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
	
	reg_status.N = (reg_a & 0b10000000) == 0b10000000;
	reg_status.V = !((reg_a ^ current_data) & 0x80) && ((reg_a ^ temp) & 0x80);

	reg_a = reg_a + current_data + reg_status.C;
	reg_status.Z = reg_a == 0x00;
}

void cpu::SBC(){
	WORD temp = reg_a - current_data - (1- reg_status.C);

	reg_status.C = current_data + !reg_status.C > reg_a;
	reg_status.N = (reg_a & 0b10000000) == 0b10000000;
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
		current_data = current_data << 1;
		bus->writeByte(abs_addr,current_data);
		reg_status.Z = current_data == 0;
		reg_status.N = (current_data & 0b10000000) >> 7;
	}else{
		reg_status.C = (reg_a & 0b10000000) >> 7;
		reg_a = reg_a << 1;
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

	bus->writeByte(0x0100 + reg_sp,hi);
	if(reg_sp > 0x00)
		reg_sp--;
	
	
	bus->writeByte(0x0100 + reg_sp,lo);
	if(reg_sp > 0x00)
		reg_sp--;

	reg_pc = abs_addr;
}

void cpu::RTS(){

	BYTE lo = bus->fetchByte(0x0100 + reg_sp);
	if(reg_sp < 0xFF)
		reg_sp++;

	BYTE hi = bus->fetchByte(0x0100 + reg_sp);
	if(reg_sp < 0xFF)
		reg_sp++;

	PLP();

	reg_pc = (hi << 8)|lo;
}

void cpu::BCC(){
	if(!reg_status.C){
		reg_pc += (signed char)current_data;
	}
}

void cpu::BCS(){
	if(reg_status.C){
		reg_pc += (signed char)current_data;
	}
}

void cpu::BEQ(){
	if(reg_status.Z){
		reg_pc += (signed char)current_data;
	}
}

void cpu::BMI(){
	if(reg_status.N){
		reg_pc += (signed char)current_data;
	}
}

void cpu::BNE(){
	if(!reg_status.Z){
		reg_pc += (signed char)current_data;
	}
}
void cpu::BPL(){
	if(!reg_status.N){
		reg_pc += (signed char)current_data;
	}
}

void cpu::BVC(){
	if(!reg_status.V){
		reg_pc += (signed char)current_data;
	}
}

void cpu::BVS(){
	if(reg_status.V){
		reg_pc += (signed char)current_data;
	}
}

void cpu::CLC() {reg_status.C = 0;}

void cpu::CLD(){reg_status.D = 0;}

void cpu::CLI(){reg_status.I = 0;}

void cpu::CLV(){reg_status.V = 0;}

void cpu::SEC(){reg_status.C = 1;}

void cpu::SED(){reg_status.D = 1;}

void cpu::SEI(){reg_status.I = 1;}

void cpu::BRK(){
	BYTE lo = reg_pc | 0x00FF;
	BYTE hi = (reg_pc | 0xFF00) >> 8;

	bus->writeByte(0x0100 + reg_sp,lo);
	if(reg_sp > 0x00)
		reg_sp--;
	
	bus->writeByte(0x0100 + reg_sp,lo);
	if(reg_sp > 0x00)
		reg_sp--;

	PHP();
	reg_pc = 0xfffe;
}

void cpu::NOP(){return;}

void cpu::RTI(){
	reg_status.setSTATUS(bus->fetchByte(0x0100+reg_sp));

	if(reg_sp < 0xFF){
		reg_sp++;
	}

	BYTE hi = bus->fetchByte(0x0100+reg_sp);

	if(reg_sp < 0xFF){
		reg_sp++;
	}

	BYTE lo = bus->fetchByte(0x0100+reg_sp);

	if(reg_sp < 0xFF){
		reg_sp++;
	}

	reg_pc = (hi<<8) | lo;
}




void cpu::reset() {
		BYTE lo = bus->fetchByte(0xFFFC);
		BYTE hi = bus->fetchByte(0xFFFD);
		reg_pc = (hi<<8) | lo;
		reg_sp = 0xFF;
		reg_a = 0x00;
		reg_x = 0x00;
		reg_y = 0x00;

		initInsSet();
}

void cpu::initInsSet(){
	m_instructionSet.resize(0xFF);

	//ADC
	m_instructionSet[0x69] = {2,&cpu::IMM,&cpu::ADC,"ADC",0x69};
	m_instructionSet[0x65] = {3,&cpu::ZP,&cpu::ADC,"ADC",0x65};
	m_instructionSet[0x75] = {4,&cpu::ZPX,&cpu::ADC,"ADC",0x75};
	m_instructionSet[0x6D] = {4,&cpu::ABS,&cpu::ADC,"ADC",0x6D};
	//m_instructionSet[0xA9] = {2,&cpu::IMM,&cpu::ADC,"ADC",0xA9};
	//m_instructionSet[0xA9] = {2,&cpu::IMM,&cpu::ADC,"ADC",0xA9};
	//m_instructionSet[0xA9] = {2,&cpu::IMM,&cpu::ADC,"ADC",0xA9};
	//m_instructionSet[0xA9] = {2,&cpu::IMM,&cpu::ADC,"ADC",0xA9};

	m_instructionSet[0xA9] = {2,&cpu::IMM,&cpu::LDA,"LDA",0xA9};
	m_instructionSet[0xA5] = {3,&cpu::ZP,&cpu::LDA,"LDA",0xA9};
	m_instructionSet[0xB5] = {4,&cpu::ZPX,&cpu::LDA,"LDA",0xA9};
	m_instructionSet[0xAD] = {4,&cpu::ABS,&cpu::LDA,"LDA",0xA9};
	m_instructionSet[0x4C] = {2,&cpu::ABS,&cpu::JMP,"JMP",0xA9};
	m_instructionSet[0x48] = {2,nullptr,&cpu::PHA,"PHA",0xA9};
}

//execute one instruction
void cpu::step() {
	//size_t cycleCount = 0;
	BYTE opcode = bus->fetchByte(reg_pc);
	reg_pc++;
	
	//cycleCount = m_instructionSet[opcode].cycleCount;

	if(m_instructionSet[opcode].addMode != nullptr){
		(this->*m_instructionSet[opcode].addMode)();
	}

	(this->*m_instructionSet[opcode].operation)();

}