all: exec

libs:  c-lib


c-lib: hexeditplus.c
	gcc -g -m32 -c -o hexeditplus.o hexeditplus.c

exec: hexeditplus.c libs
	gcc -g -m32 -c -o hexeditplus.o hexeditplus.c
	gcc -g -m32  hexeditplus.o -o hexeditplus
	rm hexeditplus.o

.PHONY: clean
clean:
	rm -rf ./*.o hexeditplus
