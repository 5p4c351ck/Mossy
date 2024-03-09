#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "memory.h"
#include "logging.h"

int main(int argc, char *argv[]){

	if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

	 FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
	
	 /*Determine file size by moving the file pointer to the end of the file
		and then assigning its position to a variable effectively getting the 
		size of the file, then we reset the file pointer back to the beggining 													
	*/

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

	/*Initialize memory and check if the file can fit in 64kb of memory,
		otherwise exit with an error message
	*/

	struct memory* mem = init_memory();
	
	if (file_size > sizeof(*mem)){
		printf("Max file size is %d bytes", sizeof(*mem));
		fclose(file);
        return 1;
	}

	size_t bytes_read = fread(mem->cell, 1, file_size, file);
    if (bytes_read != file_size) {
        perror("Error reading file");
        fclose(file);
        return 1;
    }

	struct CPU* cpu = CPU_power_on();
	CPU_reset(cpu);

	CPU_exec(cpu, mem, 100000);

	CPU_power_off(&cpu);
	free_memory(&mem);
	fclose(file);

	return 0;
}


