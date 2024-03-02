objs = cpu.o memory.o main.o
executable = 6502

all: $(executable)
	

$(executable): cpu.o memory.o main.o
	gcc $(objs) -o $(executable)
	
CPU.o: cpu.c
	gcc -g cpu.c -c -o cpu.o

memory.o: memory.c
	gcc -g memory.c -c -o memory.o

main.o: main.c
	gcc -g main.c -c -o main.o

clean:
	rm $(objs)