#ifndef MEMORY_H
#define MEMORY_H

#include "typedefs.h"

#define MEMORY_SIZE 1024 * 64

struct memory{

    byte cell[MEMORY_SIZE];
};

/*Memory API*/

extern struct memory* init_mem(void);
extern void free_mem(struct memory* mem);

#endif