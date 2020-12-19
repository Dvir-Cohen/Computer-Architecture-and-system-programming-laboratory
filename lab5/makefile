
all: myshell

myshell:  LineParser.o ProcessList.o
	gcc -g -m32 LineParser.o ProcessList.o -o myshell
	rm LineParser.o ProcessList.o
ProcessList.o: ProcessList.c
	gcc -g -O0 -m32 -Wall -c -nostdlib -fno-stack-protector ProcessList.c -o ProcessList.o

LineParser.o: LineParser.c
	gcc -g -O0 -m32 -Wall -c -nostdlib -fno-stack-protector LineParser.c -o LineParser.o


.PHONEY: clean
clean: 
	rm ./*.o myshell 