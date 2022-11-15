#pragma once
#include "device.h"

class ram : public Device {
	using BYTE = unsigned char;
	using WORD = unsigned short;


public:

	BYTE* m_pointer;

	ram(WORD startAddr, size_t range) {
		this->m_startAddr = startAddr;
		this->range = range;
		m_pointer = new BYTE[range];

		for (size_t i = 0; i < range; i++) {
			m_pointer[i] = 0x00;
		}
	}

	void clear() {
		for (size_t i = 0; i < range; i++) {
			m_pointer[i] = 0x00;
		}
	}

	// Inherited via Device
	virtual void write(WORD addr, BYTE data) override {
		if (addr >= m_startAddr && addr < m_startAddr + range)
			m_pointer[addr - m_startAddr] = data;
	};

	virtual BYTE read(WORD addr) override {
		if (addr >= m_startAddr && addr < m_startAddr + range)
			return m_pointer[addr - m_startAddr];
		return 0x00;
	};


	~ram() {
		delete[] m_pointer;
	}
};
