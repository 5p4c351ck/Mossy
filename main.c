#include <stdio.h>

#include "cpu.h"
#include "memory.h"


int main(void){

	struct memory* mem = init_mem();
	struct CPU* cpu = CPU_power_on();	

	CPU_reset(cpu);
						/*Hardcoded program for debugging purposes*/

	mem->cell[0] = 0xE6;
	mem->cell[1] = 0xA0;
	mem->cell[2] = 0xEE;
	mem->cell[3] = 0xB0;
	mem->cell[4] = 0x00;

	
	CPU_exec(cpu, mem, 11);

	CPU_state(cpu);

	printf("%d\n", mem->cell[160]);
	printf("%d\n", mem->cell[176]);

	CPU_power_off(cpu);
	free_mem(mem);

	return 0;
}
