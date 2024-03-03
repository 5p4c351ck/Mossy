#include <stdio.h>

#include "cpu.h"
#include "memory.h"


int main(void){

	struct memory* mem = init_mem();
	struct CPU* cpu = CPU_power_on();	

	CPU_reset(cpu);
						/*Hardcoded program for debugging purposes*/


	cpu->A = 0xFF;

	mem->cell[0] = 0x4A;

	mem->cell[1] = 0x46;
	mem->cell[2] = 0x0A;

	mem->cell[3] = 0x4E;
	mem->cell[4] = 0x0B;
	mem->cell[5] = 0x00;

	mem->cell[10] = 0xFF;
	mem->cell[11] = 0x00;
	
	CPU_exec(cpu, mem, 13);

	CPU_state(cpu);

	printf("0xA: %d\n", mem->cell[10]);
	printf("0xB: %d\n", mem->cell[11]);

	CPU_power_off(cpu);
	free_mem(mem);

	return 0;
}
