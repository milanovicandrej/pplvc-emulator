#pragma once
#include <stddef.h>
#include "device.h"

class stack : public Device {
	using BYTE = unsigned char;
	using WORD = unsigned short;


public:

	BYTE* m_pointer;
    BYTE sp;

	stack(WORD startAddr) {
		this->m_startAddr = startAddr;
		this->range = 256;
        this->sp = 0xFF;

		m_pointer = new BYTE[256];

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

    int push(BYTE data){
        write(m_startAddr + sp, data);
        if(sp > 0x00)
            sp--;

        return sp;
    }


	~stack() {
		delete[] m_pointer;
	}
};
