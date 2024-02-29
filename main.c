#include <stdio.h>

#include "cpu.h"
#include "memory.h"


int main(void){

	struct memory* mem = init_mem();
	struct CPU* cpu = CPU_power_on();


	CPU_reset(cpu);

	mem->cell[0] = 0xA5;
	mem->cell[1] = 0x10;
	mem->cell[16] = 0xFA;

	CPU_exec(cpu, mem, 3);

	CPU_status(cpu);

	CPU_power_off(cpu);
	free_mem(mem);

	return 0;
}
