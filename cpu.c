#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "memory.h"
#include "logging.h"


static byte zp_addr;
static word ab_addr;
static byte low;
static byte high;

struct CPU* CPU_power_on(void){

	struct CPU *cpu = malloc(sizeof(*cpu));
	if (cpu != NULL){
		return cpu;
	}
	else{
		/*Handle error if needed*/
		return NULL;
	}
}

void CPU_power_off(struct CPU **cpu){
	free((*cpu));
	(*cpu) = NULL;
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
		case CPY_IM:
			low = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_set_flags(cpu, inst, low);
		break;

		case CPY_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			low = CPU_read_byte(cpu, mem, zp_addr, &cycles);
			CPU_set_flags(cpu, inst, low);
		break;

		case CPY_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			low = CPU_read_byte(cpu, mem, ab_addr, &cycles);
			CPU_set_flags(cpu, inst, low);
		break;

		case DEC_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 2);
			mem->cell[zp_addr]--;
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, zp_addr, &cycles));
		break;

		case DEC_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 2);
			mem->cell[ab_addr]--;
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, ab_addr, &cycles));
		break;

		case DEX:
			CPU_dec_cycle(&cycles, 1);
			cpu->X--;
			CPU_set_flags(cpu, inst, cpu->X);
		break;

		case DEY:
			CPU_dec_cycle(&cycles, 1);
			cpu->Y--;
			CPU_set_flags(cpu, inst, cpu->Y);
		break;

		case EOR_IM:
			cpu->A ^= CPU_fetch_byte(cpu, mem, &cycles);
			CPU_set_flags(cpu, inst, cpu->A);
		break;

		case EOR_ZP:
			cpu->A ^= CPU_read_byte(cpu, mem, CPU_fetch_byte(cpu, mem, &cycles), &cycles);
			CPU_set_flags(cpu, inst, cpu->A);
		break;

		case EOR_AB:
			cpu->A ^= CPU_read_byte(cpu, mem, CPU_fetch_word(cpu, mem, &cycles), &cycles);
			CPU_set_flags(cpu, inst, cpu->A);
		break;

		case INC_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 1);
			mem->cell[zp_addr]++;   
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, zp_addr, &cycles));
		break;

		case INC_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 1);
			mem->cell[ab_addr]++;
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, ab_addr, &cycles));
		break;

		case INX:
			CPU_dec_cycle(&cycles, 1);
			cpu->X++;
			CPU_set_flags(cpu, inst, cpu->X);
		break;

		case INY:
			CPU_dec_cycle(&cycles, 1);
			cpu->Y++;
			CPU_set_flags(cpu, inst, cpu->Y);
		break;

		case JMP:
			cpu->PC = CPU_fetch_word(cpu, mem, &cycles);
			break;

		case JSR:
			CPU_split_word(cpu->PC + 1, &low, &high, &cycles);
			stack_push(cpu, mem, low, &cycles);
			stack_push(cpu, mem, high, &cycles);
			cpu->PC = CPU_fetch_word(cpu, mem, &cycles);
			break;
			
		case LDA_IM:
			cpu->A = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_set_flags(cpu, inst, cpu->A);
			break;
		case LDA_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			cpu->A = CPU_read_byte(cpu, mem, zp_addr, &cycles);
			CPU_set_flags(cpu, inst, cpu->A);
			break;

		case LDX_IM:
			cpu->X = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_set_flags(cpu, inst, cpu->X);
			break;
			
		case LDX_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			cpu->X = CPU_read_byte(cpu, mem, zp_addr, &cycles);
			CPU_set_flags(cpu, inst, cpu->X);
			break;

		case LDY_IM:
			cpu->Y = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_set_flags(cpu, inst, cpu->Y);
			break;

		case LDY_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			cpu->Y = CPU_read_byte(cpu, mem, zp_addr, &cycles);
			CPU_set_flags(cpu, inst, cpu->Y);
			break;
		/*LSR always sets the N flag to 0 so we dont need to use a machine cycle to check*/
		case LSR_A:
			CPU_dec_cycle(&cycles, 1);
			CPU_set_flags(cpu, inst, cpu->A);
			cpu->A >>= 1;
			break;

		case LSR_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 2);
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, zp_addr, &cycles));
			mem->cell[zp_addr] >>= 1;
			break;

		case LSR_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			CPU_dec_cycle(&cycles, 2);
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, ab_addr, &cycles));
			mem->cell[ab_addr] >>= 1;
			break;

		case NOP:
			CPU_dec_cycle(&cycles, 1);
			break;

		case RTS:
			high = stack_pop(cpu, mem, &cycles);
			low = stack_pop(cpu, mem, &cycles);
			CPU_combine_bytes(&cpu->PC, low, high, &cycles);
			CPU_dec_cycle(&cycles, 1);
			cpu->PC++;
			break;
		default:
			break;
		}
	}
}

byte CPU_fetch_byte(struct CPU * cpu, struct memory* mem, unsigned long long *cycles){
	if (cpu->PC >= 0 && cpu->PC <= sizeof(mem->cell)){
	CPU_dec_cycle(cycles, 1);
	byte inst = mem->cell[cpu->PC];
	cpu->PC += 1;
	return inst;
	}
	else{
		exit_and_save_status(cpu, OUTOFBOUNDSMEM);
	}
}

/*Two machine cycles are used to fetch a word(16bits) from memory*/
word CPU_fetch_word(struct CPU * cpu, struct memory* mem, unsigned long long *cycles){
	if (cpu->PC >= 0 && cpu->PC <= sizeof(mem->cell)){
	CPU_dec_cycle(cycles, 2);		
	word data = mem->cell[cpu->PC];		/*6502 is little endian so this is the least significant byte*/
	cpu->PC += 1;

	data |= (mem->cell[cpu->PC] << 8);	/*6502 is little endian so this is the most significant byte*/
	cpu->PC += 1;    
	return data;
	}
	else{
		exit_and_save_status(cpu, OUTOFBOUNDSMEM);
	}
}

byte CPU_read_byte(struct CPU * cpu, struct memory* mem, word addr, unsigned long long *cycles){
	if (cpu->PC >= 0 && cpu->PC <= sizeof(mem->cell)){
	CPU_dec_cycle(cycles, 1);
	byte data = mem->cell[addr];
	return data;
	}
	else{
		exit_and_save_status(cpu, OUTOFBOUNDSMEM);
	}
}

void CPU_split_word(word value, byte *low, byte *high, unsigned long long *cycles){
	CPU_dec_cycle(cycles, 1);
	(*low) = ((value) & 0xFF);
	(*high) = ((value) >> 8);
}

void CPU_combine_bytes(word *value, byte low, byte high, unsigned long long *cycles){
	CPU_dec_cycle(cycles, 2);
	(*value) = (high << 8);
	(*value) |= low;

}

static void CPU_set_flag_z(struct CPU *cpu, byte value){
	if (cpu != NULL) cpu->Z = (value == 0);
}

static void CPU_set_flag_n(struct CPU *cpu, byte value){
	if (cpu != NULL) cpu->N = (value & 0b10000000 > 0);
}

static void CPU_set_flag_c_carry(struct CPU *cpu, byte value){
	if (cpu != NULL) cpu->C = (value & 0b00000001);
}

static void CPU_set_flag_c_borrow(struct CPU *cpu, byte value){
	if (cpu != NULL){
		if (cpu->Y >= value){
				cpu->C = 1;
			}
			else{
				cpu->C = 0;
			}
	
	}
}

static void CPU_set_flags(struct CPU *cpu, byte inst, byte value){
	if (inst == CPY_IM || inst == CPY_ZP || inst == CPY_AB){
		byte result = cpu->Y - value;
		CPU_set_flag_c_borrow(cpu, value);
		CPU_set_flag_z(cpu, result);
		CPU_set_flag_n(cpu, result);
	}
	else if (inst == DEC_ZP || inst == DEC_AB ||
			 inst == DEX	|| inst == DEY    ||
			 inst == EOR_IM || inst == EOR_ZP ||
			 inst == EOR_AB || inst == INC_ZP || 
			 inst == INC_AB || inst == INX 	  || 
			 inst == INY    || inst == LDA_IM ||
			 inst == LDA_ZP || inst == LDX_IM || 
			 inst == LDX_ZP || inst == LDY_IM || 
			 inst == LDY_ZP)
	{
		CPU_set_flag_z(cpu, value);
		CPU_set_flag_n(cpu, value);
	}
	else if (inst == LSR_A  || inst == LSR_ZP || inst == LSR_AB ){
		CPU_set_flag_c_carry(cpu, value);
		cpu->N = 0;
		value >>= 8;
		CPU_set_flag_z(cpu, value);
	}
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
