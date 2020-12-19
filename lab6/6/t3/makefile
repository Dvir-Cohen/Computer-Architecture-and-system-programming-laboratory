all: exec

libs:  c-lib


c-lib: myPipe.c
		gcc -g -m32 -c -o myPipe.o myPipe.c

exec: myPipe.c libs
		gcc -g -m32 -c -o myPipe.o myPipe.c
		gcc -g -m32  myPipe.o -o myPipe
		rm myPipe.o

.PHONY: clean
clean:
		rm -rf ./*.o myPipe