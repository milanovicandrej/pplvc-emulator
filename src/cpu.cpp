#include "cpu.h"

using BYTE = unsigned char;
using WORD = unsigned short;

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

void cpu::ASL(){
		reg_status.C = (current_data & 0b10000000) >> 7;
		current_data = current_data << 1;
		bus->writeByte(abs_addr,current_data);
		reg_status.Z = current_data == 0;
		reg_status.N = (current_data & 0b10000000) >> 7;
}

void cpu::ASL_A(){
		reg_status.C = (reg_a & 0b10000000) >> 7;
		reg_a = reg_a << 1;
		reg_status.Z = reg_a == 0;
		reg_status.N = (reg_a & 0b10000000) >> 7;
}

void cpu::LSR(){
		reg_status.C = current_data & 0b00000001;
		current_data = current_data / 2;
		bus->writeByte(abs_addr,current_data);
		reg_status.Z = current_data == 0;
		reg_status.N = (current_data & 0b10000000) >> 7;
}

void cpu::LSR_A(){
		reg_status.C = reg_a & 0b10000001;
		reg_a = reg_a / 2;
		reg_status.Z = reg_a == 0;
		reg_status.N = (reg_a & 0b10000000) >> 7;
}

void cpu::ROL(){
		BYTE oldseven = (current_data & 0b10000000) >> 7;

		current_data = (current_data << 1) | reg_status.C;
		bus->writeByte(abs_addr,current_data);
		reg_status.C = oldseven;
		
		reg_status.Z = current_data == 0;
		reg_status.N = (current_data & 0b10000000) >> 7;	
}

void cpu::ROL_A(){
		BYTE oldseven = (reg_a & 0b10000000) >> 7;

		reg_a = (reg_a << 1) | reg_status.C;
		reg_status.C = oldseven;
		reg_status.Z = reg_a == 0;
		reg_status.N = (reg_a & 0b10000000) >> 7;
}

void cpu::ROR(){
		BYTE oldzero = (current_data & 0b00000001);
		
		current_data = (current_data >> 1) | (reg_status.C << 7);
		bus->writeByte(abs_addr,current_data);

		reg_status.C = oldzero;

		reg_status.Z = current_data == 0;
		reg_status.N = (reg_a & 0b10000000) >> 7;

}

void cpu::ROR_A(){
		BYTE oldzero = (reg_a & 0b00000001);
		
		reg_a = (reg_a >> 1) | (reg_status.C << 7);
		reg_status.C = oldzero;

		
		reg_status.Z = reg_a == 0;
		reg_status.N = (reg_a & 0b10000000) >> 7;
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
	
	bus->writeByte(0x0100 + reg_sp,hi);
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
	m_instructionSet[0x7D] = {4,&cpu::ABX,&cpu::ADC,"ADC",0x7D};
	m_instructionSet[0x79] = {4,&cpu::ABY,&cpu::ADC,"ADC",0x79};
	m_instructionSet[0x61] = {6,&cpu::INDX,&cpu::ADC,"ADC",0x61};
	m_instructionSet[0x71] = {5,&cpu::INDY,&cpu::ADC,"ADC",0x71};

	//AND
	m_instructionSet[0x29] = {2,&cpu::IMM,&cpu::AND,"AND",0x29};
	m_instructionSet[0x25] = {3,&cpu::ZP,&cpu::AND,"AND",0x25};
	m_instructionSet[0x35] = {4,&cpu::ZPX,&cpu::AND,"AND",0x35};
	m_instructionSet[0x2D] = {4,&cpu::ABS,&cpu::AND,"AND",0x2D};
	m_instructionSet[0x3D] = {4,&cpu::ABX,&cpu::AND,"AND",0x3D};
	m_instructionSet[0x39] = {4,&cpu::ABY,&cpu::AND,"AND",0x39};
	m_instructionSet[0x21] = {6,&cpu::INDX,&cpu::AND,"AND",0x21};
	m_instructionSet[0x31] = {5,&cpu::INDY,&cpu::AND,"AND",0x31};

	//ASL
	m_instructionSet[0x0A] = {2,nullptr,&cpu::ASL_A,"ASL",0x0A};
	m_instructionSet[0x06] = {5,&cpu::ZP,&cpu::ASL,"ASL",0x06};
	m_instructionSet[0x16] = {6,&cpu::ZPX,&cpu::ASL,"ASL",0x16};
	m_instructionSet[0x0E] = {6,&cpu::ABS,&cpu::ASL,"ASL",0x0E};
	m_instructionSet[0x1E] = {7,&cpu::ABX,&cpu::ASL,"ASL",0x1E};

	//BCC
	m_instructionSet[0x90] = {2,&cpu::REL,&cpu::BCC,"BCC",0x90};
	//BCS
	m_instructionSet[0xB0] = {2,&cpu::REL,&cpu::BCS,"BCS",0xB0};
	//BEQ
	m_instructionSet[0xF0] = {2,&cpu::REL,&cpu::BEQ,"BEQ",0xF0};
	//BIT
	m_instructionSet[0x24] = {2,&cpu::ZP,&cpu::BIT,"BIT",0x24};
	m_instructionSet[0x2C] = {2,&cpu::ABS,&cpu::BIT,"BIT",0x2C};
	//BMI
	m_instructionSet[0x30] = {2,&cpu::REL,&cpu::BMI,"BMI",0x30};
	//BNE
	m_instructionSet[0xD0] = {2,&cpu::REL,&cpu::BNE,"BNE",0xD0};
	//BPL
	m_instructionSet[0x10] = {2,&cpu::REL,&cpu::BPL,"BPL",0x10};
	//BRK
	m_instructionSet[0x00] = {7,nullptr,&cpu::BRK,"BRK",0x00};
	//BVC
	m_instructionSet[0x50] = {2,&cpu::REL,&cpu::BVC,"BVC",0x50};
	//BVS
	m_instructionSet[0x70] = {2,&cpu::REL,&cpu::BVS,"BVS",0x70};


	//CLC
	m_instructionSet[0x18] = {2,nullptr,&cpu::CLC,"CLC",0x18};
	//CLD
	m_instructionSet[0xD8] = {2,nullptr,&cpu::CLD,"CLD",0xD8};
	//CLI
	m_instructionSet[0x58] = {2,nullptr,&cpu::CLI,"CLI",0x58};
	//CLV
	m_instructionSet[0xB8] = {2,nullptr,&cpu::CLV,"CLV",0xB8};

	//CMP
	m_instructionSet[0xC9] = {2,&cpu::IMM,&cpu::CMP,"CMP",0xC9};
	m_instructionSet[0xC5] = {3,&cpu::ZP,&cpu::CMP,"CMP",0xC5};
	m_instructionSet[0xD5] = {4,&cpu::ZPX,&cpu::CMP,"CMP",0xD5};
	m_instructionSet[0xCD] = {4,&cpu::ABS,&cpu::CMP,"CMP",0xCD};
	m_instructionSet[0xDD] = {4,&cpu::ABX,&cpu::CMP,"CMP",0xDD};
	m_instructionSet[0xD9] = {4,&cpu::ABY,&cpu::CMP,"CMP",0xD9};
	m_instructionSet[0xC1] = {6,&cpu::INDX,&cpu::CMP,"CMP",0xC1};
	m_instructionSet[0xD1] = {5,&cpu::INDY,&cpu::CMP,"CMP",0xD1};

	//CPX
	m_instructionSet[0xE0] = {2,&cpu::IMM,&cpu::CPX,"CPX",0xE0};
	m_instructionSet[0xE4] = {3,&cpu::ZP,&cpu::CPX,"CPX",0xE4};
	m_instructionSet[0xEC] = {4,&cpu::ABS,&cpu::CPX,"CPX",0xEC};

	//CPY
	m_instructionSet[0xC0] = {2,&cpu::IMM,&cpu::CPY,"CPY",0xC0};
	m_instructionSet[0xC4] = {3,&cpu::ZP,&cpu::CPY,"CPY",0xC4};
	m_instructionSet[0xCC] = {4,&cpu::ABS,&cpu::CPY,"CPY",0xCC};	

	//DEC
	m_instructionSet[0xC6] = {5,&cpu::ZP,&cpu::DEC,"DEC",0xC6};
	m_instructionSet[0xD6] = {6,&cpu::ZPX,&cpu::DEC,"DEC",0xD6};
	m_instructionSet[0xCE] = {6,&cpu::ABS,&cpu::DEC,"DEC",0xCE};
	m_instructionSet[0xDE] = {7,&cpu::ABX,&cpu::DEC,"DEC",0xDE};

	//DEX
	m_instructionSet[0xCA] = {2,nullptr,&cpu::DEX,"DEX",0xCA};

	//DEY
	m_instructionSet[0x88] = {2,nullptr,&cpu::DEY,"DEY",0x88};

	//EOR
	m_instructionSet[0x49] = {2,&cpu::IMM,&cpu::EOR,"EOR",0x49};
	m_instructionSet[0x45] = {3,&cpu::ZP,&cpu::EOR,"EOR",0x45};
	m_instructionSet[0x55] = {4,&cpu::ZPX,&cpu::EOR,"EOR",0x55};
	m_instructionSet[0x4D] = {4,&cpu::ABS,&cpu::EOR,"EOR",0x4D};
	m_instructionSet[0x5D] = {4,&cpu::ABX,&cpu::EOR,"EOR",0x5D};
	m_instructionSet[0x59] = {4,&cpu::ABY,&cpu::EOR,"EOR",0x59};
	m_instructionSet[0x41] = {6,&cpu::INDX,&cpu::EOR,"EOR",0x41};
	m_instructionSet[0x51] = {5,&cpu::INDY,&cpu::EOR,"EOR",0x51};


	
	//INC
	m_instructionSet[0xE6] = {5,&cpu::ZP,&cpu::INC,"INC",0xE6};
	m_instructionSet[0xF6] = {6,&cpu::ZPX,&cpu::INC,"INC",0xF6};
	m_instructionSet[0xEE] = {6,&cpu::ABS,&cpu::INC,"INC",0xEE};
	m_instructionSet[0xFE] = {7,&cpu::ABX,&cpu::INC,"INC",0xFE};

	//INX
	m_instructionSet[0xE8] = {2,nullptr,&cpu::INX,"INX",0xE8};

	//INY
	m_instructionSet[0xC8] = {2,nullptr,&cpu::INY,"INY",0xC8};

	//JMP
	m_instructionSet[0x4C] = {3,&cpu::ABS,&cpu::JMP,"JMP",0x4C};
	m_instructionSet[0x6C] = {5,&cpu::IND,&cpu::JMP,"JMP",0x6C};


	//JSR
	m_instructionSet[0x20] = {3,&cpu::ABS,&cpu::JSR,"JSR",0x20};

	//LDA
	m_instructionSet[0xA9] = {2,&cpu::IMM,&cpu::LDA,"LDA",0xA9};
	m_instructionSet[0xA5] = {3,&cpu::ZP,&cpu::LDA,"LDA",0xA5};
	m_instructionSet[0xB5] = {4,&cpu::ZPX,&cpu::LDA,"LDA",0xB5};
	m_instructionSet[0xAD] = {4,&cpu::ABS,&cpu::LDA,"LDA",0xAD};
	m_instructionSet[0xBD] = {4,&cpu::ABX,&cpu::LDA,"LDA",0xBD};
	m_instructionSet[0xB9] = {4,&cpu::ABY,&cpu::LDA,"LDA",0xB9};
	m_instructionSet[0xA1] = {6,&cpu::INDX,&cpu::LDA,"LDA",0xA1};
	m_instructionSet[0xB1] = {5,&cpu::INDY,&cpu::LDA,"LDA",0xB1};

	//LDX
	m_instructionSet[0xA2] = {2,&cpu::IMM,&cpu::LDX,"LDX",0xA2};
	m_instructionSet[0xA6] = {3,&cpu::ZP,&cpu::LDX,"LDX",0xA6};
	m_instructionSet[0xB6] = {4,&cpu::ZPY,&cpu::LDX,"LDX",0xB6};
	m_instructionSet[0xAE] = {4,&cpu::ABS,&cpu::LDX,"LDX",0xAE};
	m_instructionSet[0xBE] = {4,&cpu::ABY,&cpu::LDX,"LDX",0xBE};

	//LDY
	m_instructionSet[0xA0] = {2,&cpu::IMM,&cpu::LDY,"LDY",0xA0};
	m_instructionSet[0xA4] = {3,&cpu::ZP,&cpu::LDY,"LDY",0xA4};
	m_instructionSet[0xB4] = {4,&cpu::ZPY,&cpu::LDY,"LDY",0xB4};
	m_instructionSet[0xAC] = {4,&cpu::ABS,&cpu::LDY,"LDY",0xAC};
	m_instructionSet[0xBC] = {4,&cpu::ABY,&cpu::LDY,"LDY",0xBC};

	//LSR
	m_instructionSet[0x4A] = {2,nullptr,&cpu::LSR_A,"LSR",0x4A};
	m_instructionSet[0x46] = {5,&cpu::ZP,&cpu::LSR,"LSR",0x46};
	m_instructionSet[0x56] = {6,&cpu::ZPX,&cpu::LSR,"LSR",0x56};
	m_instructionSet[0x4E] = {6,&cpu::ABS,&cpu::LSR,"LSR",0x4E};
	m_instructionSet[0x5E] = {7,&cpu::ABX,&cpu::LSR,"LSR",0x5E};

	//NOP
	m_instructionSet[0xEA] = {2,nullptr,&cpu::NOP,"NOP",0xEA};

	//ORA
	m_instructionSet[0x09] = {2,&cpu::IMM,&cpu::ORA,"ORA",0x09};
	m_instructionSet[0x05] = {3,&cpu::ZP,&cpu::ORA,"ORA",0x05};
	m_instructionSet[0x15] = {4,&cpu::ZPX,&cpu::ORA,"ORA",0x15};
	m_instructionSet[0x0D] = {4,&cpu::ABS,&cpu::ORA,"ORA",0x0D};
	m_instructionSet[0x1D] = {4,&cpu::ABX,&cpu::ORA,"ORA",0x1D};
	m_instructionSet[0x19] = {4,&cpu::ABY,&cpu::ORA,"ORA",0x19};
	m_instructionSet[0x01] = {6,&cpu::INDX,&cpu::ORA,"ORA",0x01};
	m_instructionSet[0x11] = {5,&cpu::INDY,&cpu::ORA,"ORA",0x11};

	//PHA
	m_instructionSet[0x48] = {3,nullptr,&cpu::PHA,"PHA",0x48};
	//PHP
	m_instructionSet[0x08] = {3,nullptr,&cpu::PHP,"PHP",0x08};
	//PLA
	m_instructionSet[0x68] = {4,nullptr,&cpu::PLA,"PLA",0x68};
	//PLP
	m_instructionSet[0x28] = {2,nullptr,&cpu::PLP,"PLP",0x28};

	//ROL
	m_instructionSet[0x2A] = {2,nullptr,&cpu::ROL_A,"ROL",0x2A};
	m_instructionSet[0x26] = {5,&cpu::ZP,&cpu::ROL,"ROL",0x26};
	m_instructionSet[0x36] = {6,&cpu::ZPX,&cpu::ROL,"ROL",0x36};
	m_instructionSet[0x2E] = {6,&cpu::ABS,&cpu::ROL,"ROL",0x2E};
	m_instructionSet[0x3E] = {7,&cpu::ABX,&cpu::ROL,"ROL",0x3E};

	//STA
	m_instructionSet[0x9D] = {4,&cpu::ABX,&cpu::STA,"STA",0x9D};

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