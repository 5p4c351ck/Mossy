#include <stdlib.h>

#include "memory.h"


struct memory* init_mem(void){

    struct memory *mem = (struct memory*)malloc(sizeof(mem));
    return mem;
}

void free_mem(struct memory* mem){

    free(mem);
}