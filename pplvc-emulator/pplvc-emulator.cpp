#include <iostream>
#include "cpu.h"
#include "ram.h"
#include "rom.h"
#include "monitor.h"

#define ADDR_SPACE 64*1024

int main()
{
	cpu cpu(1024);

	ram zeroPage(0x00, 256);
	ram stack(0x0100, 256);
	rom rom(0xE000, 8192);

	//testing
	rom.openFile("../../roms/rom_v1.bin");

	//connection
	cpu.bus->addDevice(&zeroPage);
	cpu.bus->addDevice(&stack);
	cpu.bus->addDevice(&rom);

	cpu.reset();

	Monitor mon(&cpu);
	mon.run();

}
