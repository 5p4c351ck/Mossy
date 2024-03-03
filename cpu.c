#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "memory.h"


static byte zp_addr;
static word ab_addr;
static byte low;
static byte high;

struct CPU* CPU_power_on(void){

	struct CPU *cpu = (struct CPU*)malloc(sizeof(*cpu));
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

void CPU_state(struct CPU* cpu){

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

void CPU_exec(struct CPU * cpu, struct memory* mem, unsigned long long cycles){

	while(cycles){
		/*One machine cycle is always used to fetch the next instruction(byte) from memory*/
		byte inst = CPU_fetch_byte(cpu, mem, &cycles);
		switch (inst)
		{
		case DEC_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 1);
			mem->cell[zp_addr]--;
			cpu->Z = (CPU_read_byte(cpu, mem, zp_addr, &cycles) == 0);
			cpu->N = (CPU_read_byte(cpu, mem, zp_addr, &cycles) > 0);
		break;

		case DEC_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 1);
			mem->cell[ab_addr]--;
			cpu->Z = (CPU_read_byte(cpu, mem, ab_addr, &cycles) == 0);
			cpu->N = (CPU_read_byte(cpu, mem, ab_addr, &cycles) & 0b10000000) > 0;
		break;

		case DEX:
			CPU_dec_cycle(&cycles, 1);
			cpu->X--;
			cpu->Z = (cpu->X == 0);
			cpu->N = (cpu->X & 0b10000000) > 0;
		break;

		case DEY:
			CPU_dec_cycle(&cycles, 1);
			cpu->Y--;
			cpu->Z = (cpu->Y == 0);
			cpu->N = (cpu->Y & 0b10000000) > 0;
		break;

		case EOR_IM:
			cpu->A ^= CPU_fetch_byte(cpu, mem, &cycles);
			cpu->Z = (cpu->A == 0);
			cpu->N = (cpu->A & 0b10000000) > 0;
		break;


		case EOR_ZP:
			cpu->A ^= CPU_read_byte(cpu, mem, CPU_fetch_byte(cpu, mem, &cycles), &cycles);
			cpu->Z = (cpu->A == 0);
			cpu->N = (cpu->A & 0b10000000) > 0;
		break;

		case EOR_AB:
			cpu->A ^= CPU_read_byte(cpu, mem, CPU_fetch_word(cpu, mem, &cycles), &cycles);
			cpu->Z = (cpu->A == 0);
			cpu->N = (cpu->A & 0b10000000) > 0;
		break;

		case INC_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 1);
			mem->cell[zp_addr]++;   
			cpu->Z = (CPU_read_byte(cpu, mem, zp_addr, &cycles) == 0);
			cpu->N = (CPU_read_byte(cpu, mem, zp_addr, &cycles) & 0b10000000) > 0;
		break;

		case INC_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 1);
			mem->cell[ab_addr]++;
			cpu->Z = (CPU_read_byte(cpu, mem, ab_addr, &cycles) == 0);
			cpu->N = (CPU_read_byte(cpu, mem, ab_addr, &cycles) & 0b10000000) > 0;
		break;

		case INX:
			CPU_dec_cycle(&cycles, 1);
			cpu->X++;
			cpu->Z = (cpu->X == 0);
			cpu->N = (cpu->X & 0b10000000) > 0;
		break;

		case INY:
			CPU_dec_cycle(&cycles, 1);
			cpu->Y++;
			cpu->Z = (cpu->Y == 0);
			cpu->N = (cpu->Y & 0b10000000) > 0;
		break;

		case JMP:
			cpu->PC = CPU_fetch_word(cpu, mem, &cycles);
			break;

		case JSR:
			CPU_dec_cycle(&cycles, 1);
			low = ((cpu->PC + 1) & 0xFF);
			high = ((cpu->PC + 1) >> 8);
			stack_push(cpu, mem, low, &cycles);
			stack_push(cpu, mem, high, &cycles);
			cpu->PC = CPU_fetch_word(cpu, mem, &cycles);
			break;
			
		case LDA_IM:
			cpu->A = CPU_fetch_byte(cpu, mem, &cycles);
			cpu->Z = (cpu->A == 0);
			cpu->N = (cpu->A & 0b10000000) > 0;
			break;
		case LDA_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			cpu->A = CPU_read_byte(cpu, mem, zp_addr, &cycles);
			cpu->Z = (cpu->A == 0);
			cpu->N = (cpu->A & 0b10000000) > 0;
			break;

		case LDX_IM:
			cpu->X = CPU_fetch_byte(cpu, mem, &cycles);
			cpu->Z = (cpu->X == 0);
			cpu->N = (cpu->X & 0b10000000) > 0;
			break;
			
		case LDX_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			cpu->X = CPU_read_byte(cpu, mem, zp_addr, &cycles);
			cpu->Z = (cpu->X == 0);
			cpu->N = (cpu->X & 0b10000000) > 0;
			break;

		case LDY_IM:
			cpu->Y = CPU_fetch_byte(cpu, mem, &cycles);
			cpu->Z = (cpu->Y == 0);
			cpu->N = (cpu->Y & 0b10000000) > 0;
			break;

		case LDY_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			cpu->Y = CPU_read_byte(cpu, mem, zp_addr, &cycles);
			cpu->Z = (cpu->Y == 0);
			cpu->N = (cpu->Y & 0b10000000) > 0;
			break;

		case LSR_A:
			CPU_dec_cycle(&cycles, 1);
			cpu->N = 0;
			cpu->C = cpu->A & 0b00000001;
			cpu->A >>= 1;
			cpu->Z = (cpu->A == 0);
			break;

		case LSR_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 3);
			cpu->N = 0;
			cpu->C = CPU_read_byte(cpu, mem, zp_addr, &cycles) & 0b00000001;
			mem->cell[zp_addr] >>= 1;
			cpu->Z = (CPU_read_byte(cpu, mem, zp_addr, &cycles) == 0);
			break;

		case LSR_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 3);
			cpu->N = 0;
			cpu->C = CPU_read_byte(cpu, mem, ab_addr, &cycles) & 0b00000001;
			mem->cell[ab_addr] >>= 1;
			cpu->Z = (CPU_read_byte(cpu, mem, ab_addr, &cycles) == 0);
			break;

		case NOP:
			CPU_dec_cycle(&cycles, 1);
			break;

		case RTS:
			high = stack_pop(cpu, mem, &cycles);
			low = stack_pop(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 3);
			cpu->PC = (high << 8);
			cpu->PC |= low;
			cpu->PC++;
			break;
		default:
			break;
		}
	}
}

byte CPU_fetch_byte(struct CPU * cpu, struct memory* mem, unsigned long long *cycles){

	CPU_dec_cycle(cycles, 1);
	byte inst = mem->cell[cpu->PC];
	cpu->PC += 1;
	return inst;
}

/*Two machine cycles are used to fetch a word(16bits) from memory*/
word CPU_fetch_word(struct CPU * cpu, struct memory* mem, unsigned long long *cycles){

	CPU_dec_cycle(cycles, 2);		
	word data = mem->cell[cpu->PC];		/*6502 is little endian so this is the least significant byte*/
	cpu->PC += 1;

	data |= (mem->cell[cpu->PC] << 8);	/*6502 is little endian so this is the most significant byte*/
	cpu->PC += 1;    
	return data;

}

byte CPU_read_byte(struct CPU * cpu, struct memory* mem, word addr, unsigned long long *cycles){

	CPU_dec_cycle(cycles, 1);
	byte data = mem->cell[addr];
	return data;
}

	/*
		Checking whether or not there are enough cycles left to
		finish an instruction before starting its execution
	*/

void CPU_dec_cycle(unsigned long long *cycles, unsigned short dec){

	if ((*cycles) > 0 && (*cycles) >= dec){
		(*cycles)-= dec;
	}
	else {
		exit(0);
	}
}
