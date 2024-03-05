#ifndef LOGGING_H
#define LOGGING_H

struct CPU;

enum err {
            OUTOFBOUNDSMEM,
            OUFOFBOUNDSSTACK
         };

extern void exit_and_save_status(struct CPU *cpu, enum err error);

#endif
