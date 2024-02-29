#include <stdlib.h>

#include "memory.h"
#include "cpu.h"


struct memory* init_mem(void){

    struct memory *mem = (struct memory*)malloc(sizeof(mem));
    return mem;
}

void free_mem(struct memory* mem){

    free(mem);
}

extern void stack_push(struct CPU* cpu, struct memory* mem, byte data){

    mem->cell[cpu->SP] = data;
    cpu->SP++;
}

extern byte stack_pop(struct CPU* cpu, struct memory* mem){

    byte data = mem->cell[cpu->SP];
    cpu->SP--;
    return data;
}
