#include <stdlib.h>
#include <stdio.h>

#include "memory.h"
#include "cpu.h"


const byte stack_top = 0xFF;                                                        /*The address at the top of the stack*/
const byte stack_bottom = 0x00;                                                     /*The address at the bottom of the stack*/
const word fixed_high = 0x0100;                                                     /*Fixed high byte added to stack pointer for stack access*/

static int stack_rollback = 0;                                                      /*Flag for stack wrapping*/

struct memory* init_mem(void){

    struct memory *mem = (struct memory*)malloc(sizeof(*mem));
    if (mem != NULL){
        return mem;
    }
    else{
        /*Handle error if needed*/
        return NULL;
    }
}

void free_mem(struct memory* mem){

    free(mem);
    mem = NULL;
}

extern void stack_push(struct CPU* cpu, struct memory* mem, byte data, unsigned long long *cycles){
    CPU_dec_cycle(cycles, 1);                                                                                   
    if(cpu->SP == stack_bottom) stack_rollback++;   
    mem->cell[fixed_high + cpu->SP] = data;                                           
    cpu->SP--;
}

extern byte stack_pop(struct CPU* cpu, struct memory* mem, unsigned long long *cycles){                        
    CPU_dec_cycle(cycles, 1);    
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
