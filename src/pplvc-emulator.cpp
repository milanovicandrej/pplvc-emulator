#include <iostream>
#include "cpu.h"
#include "ram.h"
#include "rom.h"
#include "monitor.h"
#include "video.h"
#include <thread>

#define ADDR_SPACE 64*1024

int main(int argc, char* argv[])
{
	cpu cpu(1024);

	ram zeroPage(0x00, 256);
	ram stack(0x0100, 256);
	ram mem(0x0200,32768);

	ram vram(0x8200,24064);

	rom rom(0xE000, 8192);


	//testing
	rom.openFile(argv[1]);

	//connection
	cpu.bus->addDevice(&zeroPage);
	cpu.bus->addDevice(&stack);
	cpu.bus->addDevice(&mem);
	cpu.bus->addDevice(&vram);
	cpu.bus->addDevice(&rom);

	cpu.reset();
	
	video vchip(&vram);
	vram.clear();

	if(vchip.Construct(256,94,1,1))
		
	
	vchip.runChip();
	Monitor mon(&cpu);
	mon.run();
	vchip.join();
}
