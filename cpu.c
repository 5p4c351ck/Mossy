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
	if (cpu != NULL){
	free((*cpu));
	(*cpu) = NULL;
	return;
}
}

void CPU_reset(struct CPU *cpu){
	if (cpu != NULL){
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
}

void CPU_state(struct CPU* cpu){
	if (cpu != NULL){
	printf("\t\t---CPU STATUS---\n");
	printf("\t\tProgram counter: %d\n", cpu->PC);
	printf("\t\tStack pointer: %d\n", cpu->SP);
	printf("\t\tRegister A: %d\n", cpu->A);
	printf("\t\tReguister X: %d\n", cpu->X);
	printf("\t\tReguister Y: %d\n", cpu->Y);
	printf("\t\tFlag C: %d\n", cpu->C);
	printf("\t\tFlag Z: %d\n", cpu->Z);
	printf("\t\tFlag I: %d\n", cpu->I);
	printf("\t\tFlag D: %d\n", cpu->D);
	printf("\t\tFlag B: %d\n", cpu->B);
	printf("\t\tFlag V: %d\n", cpu->V);
	printf("\t\tFlag N: %d\n", cpu->N);
}
}

void CPU_exec(struct CPU * cpu, struct memory* mem, unsigned long long cycles){
	if (cpu != NULL && mem != NULL){
	while(cycles){
		/*One machine cycle is always used to fetch the next instruction(byte) from memory*/
		byte inst = CPU_fetch_byte(cpu, mem, &cycles);
		switch (inst)
		{
		case CLC:
			CPU_dec_cycles(&cycles, 1);
			CPU_set_flags(cpu, inst, low);
		break;
		
		case CLD:
			CPU_dec_cycles(&cycles, 1);
			CPU_set_flags(cpu, inst, low);
		break;

		case CLI:
			CPU_dec_cycles(&cycles, 1);
			CPU_set_flags(cpu, inst, low);
		break;

		case CLV:
			CPU_dec_cycles(&cycles, 1);
			CPU_set_flags(cpu, inst, low);
		break;

		case CMP_IM: 
			low = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_set_flags(cpu, inst, low);
		break;

		case CMP_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			low = CPU_read_byte(cpu, mem, zp_addr, &cycles);
			CPU_set_flags(cpu, inst, low);
		break;

		case CMP_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			low = CPU_read_byte(cpu, mem, ab_addr, &cycles);
			CPU_set_flags(cpu, inst, low);
		break;

		case CPX_IM: 
			low = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_set_flags(cpu, inst, low);
		break;

		case CPX_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			low = CPU_read_byte(cpu, mem, zp_addr, &cycles);
			CPU_set_flags(cpu, inst, low);
		break;

		case CPX_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			low = CPU_read_byte(cpu, mem, ab_addr, &cycles);
			CPU_set_flags(cpu, inst, low);
		break;

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
			CPU_dec_cycles(&cycles, 1);
			operate_addr(cpu, mem, zp_addr, DEC, &cycles);
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, zp_addr, &cycles));
		break;

		case DEC_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			CPU_dec_cycles(&cycles, 1);
			operate_addr(cpu, mem, ab_addr, DEC, &cycles);
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, ab_addr, &cycles));
		break;

		case DEX:
			CPU_operate_reg(cpu, &cpu->X, DEC, &cycles);
			CPU_set_flags(cpu, inst, cpu->X);
		break;

		case DEY:
			CPU_operate_reg(cpu, &cpu->Y, DEC, &cycles);
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
			operate_addr(cpu, mem, zp_addr, INC, &cycles);  
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, zp_addr, &cycles));
		break;

		case INC_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			operate_addr(cpu, mem, ab_addr, INC, &cycles);
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, ab_addr, &cycles));
		break;

		case INX:
			CPU_operate_reg(cpu, &cpu->X, INC, &cycles);
			CPU_set_flags(cpu, inst, cpu->X);
		break;

		case INY:
			CPU_operate_reg(cpu, &cpu->Y, INC, &cycles);
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
			CPU_set_flags(cpu, inst, cpu->A);
			CPU_operate_reg(cpu, &cpu->A, RSH, &cycles);
			break;

		case LSR_ZP:
			zp_addr = CPU_fetch_byte(cpu, mem, &cycles);
			CPU_dec_cycles(&cycles, 1);
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, zp_addr, &cycles));
			operate_addr(cpu, mem, zp_addr, RSH, &cycles); 
			break;

		case LSR_AB:
			ab_addr = CPU_fetch_word(cpu, mem, &cycles);
			CPU_dec_cycles(&cycles, 1);
			CPU_set_flags(cpu, inst, CPU_read_byte(cpu, mem, ab_addr, &cycles));
			operate_addr(cpu, mem, ab_addr, RSH, &cycles);
			break;

		case NOP:
			CPU_dec_cycles(&cycles, 1);
			break;

		case RTS:
			high = stack_pop(cpu, mem, &cycles);
			low = stack_pop(cpu, mem, &cycles);
			CPU_combine_bytes(&cpu->PC, low, high, &cycles);
			CPU_inc_program_counter(cpu, mem, &cycles);
			break;
		default:
			break;
		}
	}
}
}

/*The following bound checking is redundant since the program counter
  cannot be negative or higher than 65535 since its a word(unsigned short)
  but will be kept as is since it doesn't cause any problems to check*/

static byte CPU_fetch_byte(struct CPU * cpu, struct memory* mem, unsigned long long *cycles){
	if (cpu != NULL){
	if (cpu->PC >= 0 && cpu->PC < sizeof(mem->cell)){
	byte inst = mem->cell[cpu->PC];
	CPU_inc_program_counter(cpu, mem, cycles);
	return inst;
	}
	else{
		exit_and_save_status(cpu, OUTOFBOUNDSMEM);
	}
}
}

/*Two machine cycles are used to fetch a word(16bits) from memory*/
static word CPU_fetch_word(struct CPU * cpu, struct memory* mem, unsigned long long *cycles){
	if (cpu != NULL){
	if (cpu->PC >= 0 && cpu->PC < sizeof(mem->cell)){	
	word data = mem->cell[cpu->PC];		/*6502 is little endian so this is the least significant byte*/
	CPU_inc_program_counter(cpu, mem, cycles);

	data |= (mem->cell[cpu->PC] << 8);	/*6502 is little endian so this is the most significant byte*/
	CPU_inc_program_counter(cpu, mem, cycles);   
	return data;
	}
	else{
		exit_and_save_status(cpu, OUTOFBOUNDSMEM);
	}
}
}

static byte CPU_read_byte(struct CPU * cpu, struct memory* mem, word addr, unsigned long long *cycles){
	if (cpu != NULL){
	if (addr >= 0 && addr < sizeof(mem->cell)){
	CPU_dec_cycles(cycles, 1);
	byte data = mem->cell[addr];
	return data;
	}
	else{
		exit_and_save_status(cpu, OUTOFBOUNDSMEM);
	}
}
}

static void CPU_split_word(word value, byte *low, byte *high, unsigned long long *cycles){
	if (low != NULL && high != NULL){
	CPU_dec_cycles(cycles, 1);
	(*low) = ((value) & 0xFF);
	(*high) = ((value) >> 8);
}
}

static void CPU_combine_bytes(word *value, byte low, byte high, unsigned long long *cycles){
	if (value != NULL){
	CPU_dec_cycles(cycles, 2);
	(*value) = (high << 8);
	(*value) |= low;

}
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
	if (cpu != NULL){
	if (inst == CPY_IM || inst == CPY_ZP || inst == CPY_AB){
		byte result = cpu->Y - value;
		CPU_set_flag_c_borrow(cpu, value);
		CPU_set_flag_z(cpu, result);
		CPU_set_flag_n(cpu, result);
	}
	else if (inst == CPX_IM || inst == CPX_ZP || inst == CPX_AB){
		byte result = cpu->X - value;
		CPU_set_flag_c_borrow(cpu, value);
		CPU_set_flag_z(cpu, result);
		CPU_set_flag_n(cpu, result);
	}
	else if (inst == CMP_IM || inst == CMP_ZP || inst == CMP_AB){
		byte result = cpu->A - value;
		CPU_set_flag_c_borrow(cpu, value);
		CPU_set_flag_z(cpu, result);
		CPU_set_flag_n(cpu, result);
	}
	else if (inst == CLC){
		cpu->C ^= cpu->C;
	}
	else if (inst == CLD){
		cpu->D ^= cpu->D;
	}
	else if (inst == CLI){
		cpu->I ^= cpu->I;
	}
	else if (inst == CLV){
		cpu->V ^= cpu->V;
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
}

static void CPU_operate_reg(struct CPU* cpu, byte *reg, enum operations oper, unsigned long long *cycles){
	if (cpu != NULL){
	if (reg != NULL){
    CPU_dec_cycles(cycles, 1);
    switch(oper){
        case DEC:
            (*reg)--;
        break;  
        case INC:
            (*reg)++;
	    break;
        case LSH:
            (*reg) <<= 1;
	    break;
        case RSH:
            (*reg) >>= 1;
        break;
	}
	}
    else{
        exit_and_save_status(cpu, OUTOFBOUNDSMEM);
    }
}
}

static void CPU_inc_program_counter(struct CPU* cpu, struct memory *mem, unsigned long long *cycles){
	if (cpu != NULL){
	if (cpu->PC < sizeof(mem->cell)){
		    CPU_dec_cycles(cycles, 1);
			cpu->PC++;
		}
	else{
       	exit_and_save_status(cpu, OUTOFBOUNDSMEM);
		}
	} 
	}

	/*
		Checking whether or not there are enough cycles left to
		finish an instruction before starting its execution
	*/

void CPU_dec_cycles(unsigned long long *cycles, unsigned short dec){
	if ((*cycles) > 0 && (*cycles) >= dec){
		(*cycles)-= dec;
	}
	else {
		exit(0);
	}
}
