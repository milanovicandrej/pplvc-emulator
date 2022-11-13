#pragma once
#include <vector>
#include "device.h"

class Bus {
	using BYTE = unsigned char;
	using WORD = unsigned short;

public:
	std::vector<Device*> devices;
	Bus() {}

	void addDevice(Device* device) {
		devices.push_back(device);
	}

	BYTE fetchByte(WORD addr) {
		for (int i = 0; i < devices.size(); i++) {
			if (addr >= devices[i]->m_startAddr && addr < devices[i]->m_startAddr + devices[i]->range) {
				return devices[i]->read(addr);
			}
		}
		return 0x00;
	}

	void writeByte(WORD addr, BYTE data) {
		for (int i = 0; i < devices.size(); i++) {
			if (addr >= devices[i]->m_startAddr && addr < devices[i]->m_startAddr + devices[i]->range) {
				devices[i]->write(addr, data);
				return;
			}
		}
	}

};