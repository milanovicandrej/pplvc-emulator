#pragma once
#pragma once
#include "device.h"
#include <fstream>


class rom : public Device {
	using BYTE = unsigned char;
	using WORD = unsigned short;

public:

	BYTE* m_pointer;

	rom(WORD startAddr, size_t range) {
		this->m_startAddr = startAddr;
		this->range = range;
		m_pointer = new BYTE[range];

		for (int i = 0; i < range; i++) {
			m_pointer[i] = 0x00;
		}
	}


	void openFile(std::string filepath) {
		std::ifstream is;
		char* buffer = new char[8192];

		is.open(filepath.c_str(), std::ios::binary);
		is.read(buffer, 8192);

		for (int i = 0; i < 8192; i++) {
			m_pointer[i] = buffer[i];
		}

		is.close();
		delete[] buffer;
 	}

	// Inherited via Device
	virtual void write(WORD addr, BYTE data) override {
		return;
	};

	virtual BYTE read(WORD addr) override {
		if (addr >= m_startAddr && addr < m_startAddr + range)
			return m_pointer[addr - m_startAddr];
		return 0x00;
	};


	~rom() {
		delete[] m_pointer;
	}
};
