#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#include "logging.h"
#include "cpu.h"

static const char* filename = "6502_CPU_LOG.txt";

void exit_and_save_status(struct CPU *cpu, enum err error){

    if (filename) {
        int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd == -1) {
            printf("Error opening log file");
            exit(0);
        }
        if (dup2(fd, STDOUT_FILENO) == -1 || dup2(fd, STDERR_FILENO) == -1) {
            printf("Error redirecting stdout/stderr");
            exit(0);
        }

        if (error == OUTOFBOUNDSMEM){
            printf("\t\tCRASH REASON: OUT OF BOUNDS MEMORY ACCESS\n\n");
        }
        else if (error == OUFOFBOUNDSSTACK){
            printf("\t\tCRASH REASON: OUT OF BOUNDS STACK ACCESS\n\n");
        }

        CPU_state(cpu);

        close(fd);

        dup2(STDOUT_FILENO, STDOUT_FILENO);
        dup2(STDERR_FILENO, STDERR_FILENO);
    }

    exit(0);
}