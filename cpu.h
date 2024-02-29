#ifndef CPU_H
#define CPU_H

/*Instructions and opcodes*/

#define LDA_IM	0xA9				/*Immediate addressing*/
#define LDA_ZP	0xA5				/*Zero page addressing*/

#define LDA_AB	0xAD				/*Absolute addressing*/
#define LDX_AB	0xAE				/*Absolute addressing*/
#define LDY_AB	0xAC				/*Absolute addressing*/

#include "typedefs.h"
#include "memory.h"

struct CPU{

word PC; 							/*Program counter*/
byte SP; 							/*Stack pointer*/

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
extern void CPU_exec(struct CPU * cpu, struct memory* mem, unsigned int cycles);
static byte CPU_fetch(struct CPU * cpu, struct memory* mem, unsigned int *cycles);				/*Fetch instruction from memory*/
static byte CPU_read(struct CPU * cpu, struct memory* mem, byte addr, unsigned int *cycles); 	/*Similar to fetch but doesn't increment program counter*/
#endif
