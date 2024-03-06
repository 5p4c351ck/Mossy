#include <stdlib.h>
#include <stdio.h>

#include "memory.h"
#include "cpu.h"
#include "logging.h"


static const byte stack_top = 0xFF;                                                        /*The address at the top of the stack*/
static const byte stack_bottom = 0x00;                                                     /*The address at the bottom of the stack*/
static const word fixed_high = 0x0100;                                                     /*Fixed high byte added to stack pointer for stack access*/
static const word intenal_stack_top = fixed_high + stack_top;                              /*The actual top memory address of the stack*/
static const word intenal_stack_bottom = fixed_high + stack_bottom;                        /*The actual bottom memory address of the stack*/
static int stack_rollback = 0;                                                             /*Flag for stack wrapping*/

struct memory* init_memory(void){

    struct memory *mem = malloc(sizeof(*mem));
    if (mem != NULL){
        return mem;
    }
    else{
        /*Handle error if needed*/
        return NULL;
    }
}

void free_memory(struct memory** mem){

    free((*mem));
    (*mem) = NULL;
}

/*
  The following bound checking is quite redundant,
  and will only fail if something changes the stack referencing
  values, but it will be kept as is for the moment
*/

extern void stack_push(struct CPU* cpu, struct memory* mem, byte data, unsigned long long *cycles){
    if ((fixed_high + cpu->SP) >= intenal_stack_bottom && (fixed_high + cpu->SP) <= intenal_stack_top){
    CPU_dec_cycles(cycles, 1);                                                                                   
    if (cpu->SP == stack_bottom) stack_rollback++;
    mem->cell[fixed_high + cpu->SP] = data;                                           
    cpu->SP--;
    }
    else{
        exit_and_save_status(cpu, OUFOFBOUNDSSTACK);
    }
}

extern byte stack_pop(struct CPU* cpu, struct memory* mem, unsigned long long *cycles){   
    if ((fixed_high + cpu->SP) >= intenal_stack_bottom && (fixed_high + cpu->SP) <= intenal_stack_top){                     
    CPU_dec_cycles(cycles, 1);    
    if(cpu->SP == stack_top && stack_rollback == 0){
            printf("Stack is empty\n"); /*Left here for debugging purposes to be removed later*/
    }
    else{
        cpu->SP++;
        byte data = mem->cell[fixed_high + cpu->SP];
        mem->cell[fixed_high + cpu->SP] = 0;
        return data;
    }
}
else{
    exit_and_save_status(cpu, OUFOFBOUNDSSTACK);
}
}