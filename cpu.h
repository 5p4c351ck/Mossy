#ifndef CPU_H
#define CPU_H

#include "typedefs.h"


/*Instructions and opcodes*/


#define	INC_ZP	0xE6					/*Increment Memory by One  		zero page addressing	2 bytes		5 cycles*/
#define	INC_AB	0xEE					/*Increment Memory by One		absolute  addressing	3 bytes		6 cycles*/
#define	INX		0xE8					/*Increment Index X by One 		implied   addressing	1 bytes		2 cycles*/
#define	INY		0xC8					/*Increment Index Y by One 		implied   addressing	1 bytes		2 cycles*/
#define	JMP		0x4C					/*Jump 			 		   		absolute  addressing	3 bytes		3 cycles*/
#define	JSR		0x20					/*Jump to subroutine  		   	absolute  addressing	3 bytes		6 cycles*/
#define LDA_IM	0xA9					/*Load accumulator 		   		immediate addressing	2 bytes		2 cycles*/
#define LDA_ZP	0xA5					/*Load accumulator 		   		zero page addressing	2 bytes		3 cycles*/
#define LDX_IM	0xA2					/*Register X	 	       		immediate addressing	2 bytes		3 cycles*/
#define LDX_ZP	0xA6					/*Register X 	  		   		zero page addressing	2 bytes		3 cycles*/
#define LDY_IM	0xA0					/*Register Y	 	  	   		immediate addressing	2 bytes		3 cycles*/
#define LDY_ZP	0xA4					/*Register Y	 	  	   		zero page addressing	2 bytes		3 cycles*/
#define NOP		0xEA					/*No operation	   		   		implied   addressing	1 byte 		2 cycles*/
#define RTS		0x60					/*Return from subroutine   		implied   addressing	1 byte 		6 cycles*/

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
