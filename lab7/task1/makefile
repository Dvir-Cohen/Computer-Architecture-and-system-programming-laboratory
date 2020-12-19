all: exec

libs:  c-lib


c-lib: hexeditplus.c
	gcc -g -m32 -c -o hexeditplus.o hexeditplus.c
	gcc -g -m32 -fno-pie -c -o counter.o counter.c

exec: hexeditplus.c libs
	gcc -g -m32  hexeditplus.o -o hexeditplus
	gcc -g -m32  -fno-pie counter.o -o counter
	rm hexeditplus.o counter.o

.PHONY: clean
clean:
	rm -rf ./*.o hexeditplus
