all: exec

libs: asm-lib c-lib

asm-lib: asm_task2Assignment1.s
	# flags meanings:
	# -g = debug symbolx
	# -f elf = output format (ELF 32 bit)
	# -o asm_task2Assignment1.o = write assembled file to asm_task2Assignment1.o
	nasm -g -f elf -o asm_task2Assignment1.o asm_task2Assignment1.s

c-lib: main_task2Assignment1.c
	# flags meanings:
	# -g = debug symbols
	# -m32 = compile to x86_32 architecture (32bit)
	# -c = Don't link (i.e., create an object file, not an executable)
	# -o main_task2Assignment1.o = output compiled file to main_task2Assignment1.o
	gcc -g -m32 -c -o main_task2Assignment1.o main_task2Assignment1.c

exec: main_task2Assignment1.c libs
	gcc -g -m32 -c -o main_task2Assignment1.o main_task2Assignment1.c
	gcc -g -m32  main_task2Assignment1.o asm_task2Assignment1.o -o main
	rm main_task2Assignment1.o asm_task2Assignment1.o

.PHONY: clean
clean:
	rm -rf ./*.o main
