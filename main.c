#include <stdio.h>

#include "cpu.h"
#include "memory.h"


int main(void){

	struct memory* mem = init_mem();
	struct CPU* cpu = CPU_power_on();	

	CPU_reset(cpu);
						/*Hardcoded program for debugging instructions*/
	mem->cell[0] = 0x20;
	mem->cell[1] = 0x0A;
	mem->cell[2] = 0x00;

	mem->cell[10] = 0xA9;
	mem->cell[11] = 0xFF;

	CPU_exec(cpu, mem, 8);

	CPU_status(cpu);

	CPU_power_off(cpu);
	free_mem(mem);

	return 0;
}
