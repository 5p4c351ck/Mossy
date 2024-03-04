#ifndef MEMORY_H
#define MEMORY_H

#include "typedefs.h"

struct CPU;

#define MEMORY_SIZE 1024 * 64

struct memory{

    byte cell[MEMORY_SIZE];
};

/*Memory API*/

extern struct memory* init_mem(void);
extern void free_mem(struct memory** mem);
extern void stack_push(struct CPU* cpu, struct memory* mem, byte data, unsigned long long *cycles);
extern byte stack_pop(struct CPU*, struct memory* mem, unsigned long long *cycles);


#endif
