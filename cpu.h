#ifndef CPU_H
#define CPU_H

#include "typedefs.h"


/*Instructions and opcodes*/

#define LDA_IM	0xA9					/*Load accumulator -  immediate addressing - 2 bytes - 2 cycles*/
#define LDA_ZP	0xA5					/*Load accumulator -  zero page addressing - 2 bytes - 3 cycles*/

#define	JSR		0x20				/*Jump subroutine -  absolute addressing - 3 bytes - 6 cycles*/

#define RTS		0x60				/*Return from subroutine - implied addressing - 1 byte - 6 cycles*/

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
extern void CPU_status(struct CPU*);
extern void CPU_exec(struct CPU * cpu, struct memory* mem, unsigned long long cycles);
static byte CPU_fetch_byte(struct CPU * cpu, struct memory* mem, unsigned long long *cycles);				/*Fetch instruction(8bits) from program counter address*/
static word CPU_fetch_word(struct CPU * cpu, struct memory* mem, unsigned long long *cycles);				/*Fetch 16bits from program counter address*/
static byte CPU_read_byte(struct CPU * cpu, struct memory* mem, byte addr, unsigned long long *cycles); 		/*Read 8bits from memory address*/
static void CPU_dec_cycle(unsigned long long *cycles, unsigned short dec);
#endif
