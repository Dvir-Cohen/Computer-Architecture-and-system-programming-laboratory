
all: myshell

myshell:  mypipe.o 
	gcc -g -m32  mypipe.o -o mypipe
	rm mypipe.o 
mypipe.o: mypipe.c
	gcc -g -O0 -m32 -Wall -c -nostdlib -fno-stack-protector mypipe.c -o mypipe.o


.PHONEY: clean
clean: 
	rm ./*.o mypipe 