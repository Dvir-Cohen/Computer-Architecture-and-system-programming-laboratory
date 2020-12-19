all: exec

libs:  c-lib


c-lib: menu.c
	# flags meanings:
	# -g = debug symbols
	# -m32 = compile to x86_32 architecture (32bit)
	# -c = Don't link (i.e., create an object file, not an executable)
	# -menu.o = output compiled file to menu.o
	

exec: menu.c libs
	gcc -g -m32 -c -o menu.o menu.c
	gcc -g -m32  menu.o -o menu
	rm menu.o

.PHONY: clean
clean:
	rm -rf ./*.o menu
