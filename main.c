#include <stdio.h>

#include "cpu.h"
#include "memory.h"


int main(void){

	struct memory* mem = init_mem();
	struct CPU* cpu = CPU_power_on();	

	CPU_reset(cpu);

						/*Hardcoded program for debugging purposes*/
	cpu->A = 0xFF;

	mem->cell[0] = 0x49;
	mem->cell[1] = 0x00;
	
	mem->cell[2] = 0x45;
	mem->cell[3] = 0x0A;

	mem->cell[4] = 0x4D;
	mem->cell[5] = 0x0B;
	mem->cell[6] = 0x00;	

	mem->cell[10] = 0x00;
	mem->cell[11] = 0xFF;

	CPU_exec(cpu, mem, 9);

	CPU_state(cpu);

	CPU_power_off(&cpu);
	free_mem(&mem);

	return 0;
}
