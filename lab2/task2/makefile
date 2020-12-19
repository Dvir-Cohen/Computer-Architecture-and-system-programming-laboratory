all: exec

libs:  c-lib


c-lib: task2.c
	# flags meanings:
	# -g = debug symbols
	# -m32 = compile to x86_32 architecture (32bit)
	# -c = Don't link (i.e., create an object file, not an executable)
	# -task2.o = output compiled file to task2.o
	

exec: task2.c libs
	gcc -g -m32 -c -o task2.o task2.c
	gcc -g -m32  task2.o -o task2
	rm task2.o

.PHONY: clean
clean:
	rm -rf ./*.o task2
