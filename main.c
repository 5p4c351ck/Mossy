#include <stdio.h>

#include "cpu.h"
#include "memory.h"


int main(void){

	struct memory* mem = init_mem();
	struct CPU* cpu = CPU_power_on();	

	CPU_reset(cpu);
						/*Hardcoded program for debugging instructions*/

	mem->cell[0] = 0xEA;
	mem->cell[1] = 0xEA;
	mem->cell[2] = 0xEA;

	CPU_exec(cpu, mem, 2);

	CPU_status(cpu);

	CPU_power_off(cpu);
	free_mem(mem);

	return 0;
}
