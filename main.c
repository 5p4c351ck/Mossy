#include <stdio.h>

#include "cpu.h"
#include "memory.h"
#include "logging.h"

int main(void){
	
	struct memory* mem = init_memory();
	struct CPU* cpu = CPU_power_on();	
	
	CPU_reset(cpu);

						/*Hardcoded program for debugging purposes*/
	cpu->Y = 0xFF;

	mem->cell[0] = 0xC0;
	mem->cell[1] = 0xFF;

	CPU_exec(cpu, mem, 2);

	CPU_state(cpu);

	CPU_power_off(&cpu);
	free_memory(&mem);

	return 0;
}
