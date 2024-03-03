#ifndef CPU_H
#define CPU_H

#include "typedefs.h"


/*Instructions and opcodes*/

/*				OPCODE						DESCTRIPTION				ADDRESSING	 	 BYTES		  CYCLES			*/

#define	INC_ZP	0xE6					/*Increment Memory by One  		Zero page		2 bytes		5 cycles*/
#define	INC_AB	0xEE					/*Increment Memory by One		Absolute 		3 bytes		6 cycles*/
#define	INX		0xE8					/*Increment Index X by One 		Implied  		1 bytes		2 cycles*/
#define	INY		0xC8					/*Increment Index Y by One 		Implied  		1 bytes		2 cycles*/
#define	JMP		0x4C					/*Jump 			 		   		Absolute 		3 bytes		3 cycles*/
#define	JSR		0x20					/*Jump to subroutine  		   	Absolute 		3 bytes		6 cycles*/
#define LDA_IM	0xA9					/*Load accumulator 		   		Immediate		2 bytes		2 cycles*/
#define LDA_ZP	0xA5					/*Load accumulator 		   		Zero page		2 bytes		3 cycles*/
#define LDX_IM	0xA2					/*Register X	 	       		Immediate		2 bytes		3 cycles*/
#define LDX_ZP	0xA6					/*Register X 	  		   		Zero page		2 bytes		3 cycles*/
#define LDY_IM	0xA0					/*Register Y	 	  	   		Immediate		2 bytes		3 cycles*/
#define LDY_ZP	0xA4					/*Register Y	 	  	   		Zero page		2 bytes		3 cycles*/
#define LSR_A	0x4A					/*No operation	   		   		Accumulator 	1 byte 		2 cycles*/
#define LSR_ZP	0x46					/*No operation	   		   		Zero page   	2 byte 		5 cycles*/
#define LSR_AB	0x4E					/*No operation	   		   		Absolute    	3 byte 		6 cycles*/
#define NOP		0xEA					/*No operation	   		   		Implied   		1 byte 		2 cycles*/
#define RTS		0x60					/*Return from subroutine   		Implied   		1 byte 		6 cycles*/

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

extern struct CPU* CPU_power_on(void);
extern void CPU_power_off(struct CPU * cpu);
extern void CPU_reset(struct CPU * cpu);
extern void CPU_state(struct CPU*);
extern void CPU_exec(struct CPU * cpu, struct memory* mem, unsigned long long cycles);
static byte CPU_fetch_byte(struct CPU * cpu, struct memory* mem, unsigned long long *cycles);				/*Fetch instruction(8bits) from program counter address*/
static word CPU_fetch_word(struct CPU * cpu, struct memory* mem, unsigned long long *cycles);				/*Fetch 16bits from program counter address*/
static byte CPU_read_byte(struct CPU * cpu, struct memory* mem, byte addr, unsigned long long *cycles); 		/*Read 8bits from memory address*/
static void CPU_dec_cycle(unsigned long long *cycles, unsigned short dec);
#endif
