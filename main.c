#include <stdio.h>

#include "cpu.h"
#include "memory.h"


int main(void){

	struct memory* mem = init_mem();
	struct CPU* cpu = CPU_power_on();	

	CPU_reset(cpu);
						/*Hardcoded program for debugging instructions*/

	mem->cell[0] = 0x20;
	mem->cell[1] = 0xA0;

	mem->cell[160] = 0x60;

	CPU_exec(cpu, mem, 12);

	CPU_state(cpu);

	CPU_power_off(cpu);
	free_mem(mem);

	return 0;
}
