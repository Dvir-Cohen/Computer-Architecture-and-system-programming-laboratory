all: exec

libs:  c-lib


c-lib: counter.c
	gcc -g -m32 -fno-pie -c -o counter.o counter.c

exec: counter.c libs
	gcc -g -m32 -fno-pie -c -o counter.o counter.c
	gcc -g -m32  -fno-pie counter.o -o counter
	rm counter.o

.PHONY: clean
clean:
	rm -rf ./*.o counter
