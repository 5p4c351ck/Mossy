objs = cpu.o memory.o main.o logging.o
executable = 6502

all: $(executable)
	

$(executable): cpu.o memory.o main.o logging.o
	gcc $(objs) -o $(executable)
	
CPU.o: cpu.c
	gcc -g cpu.c -c -o cpu.o

memory.o: memory.c
	gcc -g memory.c -c -o memory.o

logging.o: logging.c
	gcc -g logging.c -c -o logging.o

main.o: main.c
	gcc -g main.c -c -o main.o

clean:
	rm $(objs)