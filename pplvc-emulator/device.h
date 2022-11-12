#pragma once
class Device{
	using BYTE = unsigned char;
	using WORD = unsigned short;

public:
	WORD m_startAddr;
	size_t range;
	virtual void write(WORD addr, BYTE data) = 0;
	virtual BYTE read(WORD addr) = 0;

};

