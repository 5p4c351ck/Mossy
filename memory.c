#include <stdlib.h>
#include <stdio.h>

#include "memory.h"
#include "cpu.h"

static int stack_rollback = 0;                                                      /*Flag for stack wrapping*/

struct memory* init_mem(void){

    struct memory *mem = (struct memory*)malloc(sizeof(*mem));
    return mem;
}

void free_mem(struct memory* mem){

    free(mem);
}

extern void stack_push(struct CPU* cpu, struct memory* mem, byte data){            /*Take into account the fixed high byte 0x01*/
                                                                                     
    if(cpu->SP == 0x00) stack_rollback++;   
    mem->cell[0x0100 + cpu->SP] = data;                                           
    cpu->SP--;
}

extern byte stack_pop(struct CPU* cpu, struct memory* mem){                        
        
    if(cpu->SP == 0xFF && stack_rollback == 0){
            printf("Stack is empty\n");
    }
    else{
        cpu->SP++;
        byte data = mem->cell[0x0100 + cpu->SP];
        mem->cell[0x0100 + cpu->SP] = 0x00;
        return data;
    }
}
