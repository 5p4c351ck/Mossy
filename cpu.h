#ifndef CPU_H
#define CPU_H

#include "typedefs.h"


/*Instructions and opcodes*/

/*	       OPCODE				   DESCTRIPTION				ADDRESSING	 	 BYTES		  CYCLES			*/


#define	 	BVS		0x70		/*branch on V = 1			Relative		2 bytes		2 cycles  + 1 same page + 2 diff page */
#define		CLC		0x18		/*Clear Carry Flag			Implied			1 bytes		2 cycles*/
#define		CLD		0xD8		/*Clear Decimal Mode			Implied			1 bytes		2 cycles*/
#define		CLI		0x58		/*Clear Interrupt Disable Bit		Implied			1 bytes		2 cycles*/
#define		CLV		0xB8		/*Clear Overflow Flag			Implied			1 bytes		2 cycles*/
#define		CMP_IM		0xC9		/*Compare Memory with Accumulator	Immediate		2 bytes		2 cycles*/
#define		CMP_ZP		0xC5 		/*Compare Memory with Accumulator	Zero page		2 bytes		3 cycles*/
#define		CMP_AB		0xCD		/*Compare Memory with Accumulator	Absolute		3 bytes		4 cycles*/
#define		CPX_IM		0xE0		/*Compare Memory and Index X		Immediate		2 bytes		2 cycles*/
#define		CPX_ZP		0xE4		/*Compare Memory and Index X		Zero page		2 bytes		3 cycles*/
#define		CPX_AB		0xEC		/*Compare Memory and Index X		Absolute		3 bytes		4 cycles*/
#define		CPY_IM		0xC0		/*Compare Memory and Index Y		Immediate		2 bytes		2 cycles*/
#define		CPY_ZP		0xC4		/*Compare Memory and Index Y		Zero page		2 bytes		3 cycles*/
#define		CPY_AB		0xCC		/*Compare Memory and Index Y		Absolute		3 bytes		4 cycles*/
#define		DEC_ZP		0xC6		/*Decrement Memory by One		Zero page		2 bytes		5 cycles*/
#define		DEC_AB		0xCE		/*Decrement Memory by One		Absolute		3 bytes		6 cycles*/
#define		DEX		0xCA		/*Decrement Index X by One		Implied			1 bytes		2 cycles*/
#define		DEY		0x88		/*Decrement Index Y by One		Implied			1 bytes		2 cycles*/	
#define		EOR_IM		0x49		/*XOR Accumulator			Immediate		2 bytes		2 cycles*/
#define		EOR_ZP		0x45		/*XOR Accumulator  			Zero page		2 bytes		3 cycles*/
#define		EOR_AB		0x4D		/*XOR Accumulator 			Absolute		3 bytes		4 cycles*/
#define		INC_ZP		0xE6		/*Increment Memory by One  		Zero page		2 bytes		5 cycles*/
#define		INC_AB		0xEE		/*Increment Memory by One		Absolute 		3 bytes		6 cycles*/
#define		INX		0xE8		/*Increment Index X by One 		Implied  		1 bytes		2 cycles*/
#define		INY		0xC8		/*Increment Index Y by One 		Implied  		1 bytes		2 cycles*/
#define		JMP		0x4C		/*Jump 			 		Absolute 		3 bytes		3 cycles*/
#define		JSR		0x20		/*Jump to subroutine  		   	Absolute 		3 bytes		6 cycles*/
#define 	LDA_IM		0xA9		/*Load accumulator 		   	Immediate		2 bytes		2 cycles*/
#define 	LDA_ZP		0xA5		/*Load accumulator 		   	Zero page		2 bytes		3 cycles*/
#define 	LDX_IM		0xA2		/*Register X	 	       		Immediate		2 bytes		3 cycles*/
#define 	LDX_ZP		0xA6		/*Register X 	  		   	Zero page		2 bytes		3 cycles*/
#define 	LDY_IM		0xA0		/*Register Y	 	  	   	Immediate		2 bytes		3 cycles*/
#define 	LDY_ZP		0xA4		/*Register Y	 	  	   	Zero page		2 bytes		3 cycles*/
#define 	LSR_A		0x4A		/*No operation	   		   	Accumulator 		1 byte 		2 cycles*/
#define 	LSR_ZP		0x46		/*No operation	   		   	Zero page   		2 byte 		5 cycles*/
#define 	LSR_AB		0x4E		/*No operation	   		   	Absolute    		3 byte 		6 cycles*/
#define 	NOP		0xEA		/*No operation	   		   	Implied   		1 byte 		2 cycles*/
#define 	RTS		0x60		/*Return from subroutine   		Implied   		1 byte 		6 cycles*/

enum operations;

struct memory;

struct CPU{

word PC; 						/*Program counter*/
byte SP; 						/*Stack pointer*/

	/*General purpose registers*/			
byte A;					
byte X;					
byte Y;					
	/*Processor status flags*/
				
byte C : 1; 						/*Carry flag*/
byte Z : 1; 						/*Zero flag*/
byte I : 1; 						/*Interrupt disable*/
byte D : 1; 						/*Decimal mode*/
byte B : 1; 						/*Break command*/
byte V : 1; 						/*Overflow flag*/
byte N : 1; 						/*Negative flag*/

};

/*CPU API*/

extern struct CPU* CPU_power_on(void);											/*Powers on the virtual CPU*/
extern void CPU_power_off(struct CPU **cpu);										/*Powers off the virtual CPU*/
extern void CPU_reset(struct CPU *cpu);											/*Resets the state of the CPU*/
extern void CPU_state(struct CPU* cpu);											/*Prints the state of the CPU*/
extern void CPU_exec(struct CPU *cpu, struct memory *mem, unsigned long long cycles);					/*Starts executing instructions*/

/*CPU API Internal subroutines(not to be called manually)*/

static byte CPU_fetch_byte(struct CPU *cpu, struct memory *mem, unsigned long long *cycles);				/*Fetch instruction(8bits) from program counter address*/
static word CPU_fetch_word(struct CPU *cpu, struct memory *mem, unsigned long long *cycles);				/*Fetch 16bits from program counter address*/
static byte CPU_read_byte(struct CPU *cpu, struct memory *mem, word addr, unsigned long long *cycles); 			/*Read 8bits from memory address*/
static void CPU_split_word(word value, byte *low, byte *high, unsigned long long *cycles);				/*Splits a word into two bytes*/
static void CPU_combine_bytes(word *value, byte low, byte high, unsigned long long *cycles);				/*Combines two bytes into a word*/
static void CPU_set_flag_z(struct CPU *cpu, byte value);								
static void CPU_set_flag_n(struct CPU *cpu, byte value);	
static void CPU_set_flag_c_carry(struct CPU *cpu, byte value);
static void CPU_set_flag_c_borrow(struct CPU *cpu, byte value);
static void CPU_set_flags(struct CPU *cpu, byte inst, byte value);
static void CPU_operate_reg(struct CPU *cpu, byte *reg, enum operations oper, unsigned long long *cycles);
static void CPU_inc_program_counter(struct CPU* cpu, struct memory *mem, unsigned long long *cycles);
extern void CPU_dec_cycles(unsigned long long *cycles, unsigned short dec);
#endif
