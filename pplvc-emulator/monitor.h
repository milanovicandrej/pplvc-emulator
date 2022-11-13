#pragma once
#include "cpu.h"
#include <string>

#define HEX( x ) std::setw(2) << std::setfill('0') << std::hex << (int)(x)

std::vector<std::string> split(std::string str, std::string token) {
	std::vector<std::string>result;
	while (str.size()) {
		int index = str.find(token);
		if (index != std::string::npos) {
			result.push_back(str.substr(0, index));
			str = str.substr(index + token.size());
			if (str.size() == 0)result.push_back(str);
		}
		else {
			result.push_back(str);
			str = "";
		}
	}
	return result;
}


class Monitor {
	using BYTE = unsigned char;
	using WORD = unsigned short;

public:

	cpu* processor;
	Monitor(cpu * processor) {
		this->processor = processor;
	}

	void run() {
		std::cout << "PPLVC Computer monitor" << std::endl << "Please insert command:" << std::endl;


		while (true) {
		
			//input command
			std::string str;

			std::vector<std::string> command;
			
			std::getline(std::cin, str);

			if (str == "") {
				std::cout <<"Please insert a valid command !"<< std::endl;
				continue;
			}

			command = split(str, " ");

			if (command[0] == "exit") {
				return;
			}
			else if (command[0] == "hd") {
				WORD start = std::stoul(command[1], nullptr, 16);
				WORD end = std::stoul(command[2], nullptr, 16);
				hexDump(start, end);
			}
			else if(command[0] == "step"){
				step();
			}
			else if(command[0] == "a"){
				std::cout<<"REG_A : "<<HEX(processor->reg_a)<<std::endl;
			}
			else if(command[0] == "pc"){
				std::cout<<"PC : "<<HEX(processor->reg_pc)<<" -> ("<<HEX(processor->bus->fetchByte(processor->reg_pc))<<")"<<std::endl;
			}
			else {
				std::cout << "Command " << command[0] << " is invalid" << std::endl;
			}
		}

	}

	void step(){
		processor->step();
	}

	void hexDump(WORD start, WORD end) {
		int counter = 0;
		for (WORD i = start; i < end; i++) {
			BYTE b = processor->bus->fetchByte(i);
			if (counter == 0) {
				std::cout << HEX(i) << " ";
			}
			std::cout << HEX(b) << " ";
			counter++;

			if (counter == 10) {
				std::cout << std::endl;
				counter = 0;
			}
		}
		BYTE b = processor->bus->fetchByte(end);
		std::cout << HEX(b) << " " << std::endl;
	}
};