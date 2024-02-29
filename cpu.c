#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

struct CPU* CPU_power_on(void){

	struct CPU *cpu = (struct CPU*)malloc(sizeof(cpu));
	return cpu;
}

void CPU_power_off(struct CPU *cpu){
	free(cpu);
	return;
}

void CPU_reset(struct CPU *cpu){

	cpu->PC = 0x00;
	cpu->SP = 0xFF;

	cpu->C = 0;
	cpu->Z = 0;
	cpu->I = 0;
	cpu->D = 0;
	cpu->B = 0;
	cpu->V = 0;
	cpu->N = 0;
	
	cpu->A = 0;
	cpu->X = 0;
	cpu->Y = 0;
	
	return;
}

void CPU_status(struct CPU* cpu){

	printf("\n---CPU STATUS---\n");
	printf("Program counter: %d\n", cpu->PC);
	printf("Stack pointer: %d\n", cpu->SP);
	printf("Register A: %d\n", cpu->A);
	printf("Reguister X: %d\n", cpu->X);
	printf("Reguister Y: %d\n", cpu->Y);
	printf("Flag C: %d\n", cpu->C);
	printf("Flag Z: %d\n", cpu->Z);
	printf("Flag I: %d\n", cpu->I);
	printf("Flag D: %d\n", cpu->D);
	printf("Flag B: %d\n", cpu->B);
	printf("Flag V: %d\n", cpu->V);
	printf("Flag N: %d\n", cpu->N);
}

void CPU_exec(struct CPU * cpu, struct memory* mem, unsigned int cycles){

	while(cycles > 0){

		byte inst = CPU_fetch(cpu, mem, &cycles);
		switch (inst)
		{
		case LDA_IM:
			cpu->A = CPU_fetch(cpu, mem, &cycles);
			cpu->Z = (cpu->A == 0);
			cpu->N = (cpu->A & 0b10000000) > 0;
			break;
		case LDA_ZP:
			byte zp_addr = CPU_fetch(cpu, mem, &cycles);
			cpu->A = CPU_read(cpu, mem, zp_addr, &cycles);
			cpu->Z = (cpu->A == 0);
			cpu->N = (cpu->A & 0b10000000) > 0;
			break;
		default:
			break;
		}
	}
}

byte CPU_fetch(struct CPU * cpu, struct memory* mem, unsigned int *cycles){

	byte inst = mem->cell[cpu->PC];
	cpu->PC += 1;
	(*cycles) -= 1;
	return inst;
}

byte CPU_read(struct CPU * cpu, struct memory* mem, byte addr, unsigned int *cycles){

	byte data = mem->cell[addr];
	(*cycles) -= 1;
	return data;
}