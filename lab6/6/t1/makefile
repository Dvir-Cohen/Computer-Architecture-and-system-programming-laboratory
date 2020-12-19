
all: myshell

myshell:  LineParser.o myshell.o
	gcc -g -m32 LineParser.o myshell.o -o myshell
	rm LineParser.o myshell.o
myshell.o: myshell.c
	gcc -g -O0 -m32 -Wall -c -nostdlib -fno-stack-protector myshell.c -o myshell.o

LineParser.o: LineParser.c
	gcc -g -O0 -m32 -Wall -c -nostdlib -fno-stack-protector LineParser.c -o LineParser.o


.PHONEY: clean
clean: 
	rm ./*.o myshell 