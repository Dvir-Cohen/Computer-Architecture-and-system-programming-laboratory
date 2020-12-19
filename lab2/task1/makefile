all: exec

libs:  c-lib


c-lib: addresses.c
	# flags meanings:
	# -g = debug symbols
	# -m32 = compile to x86_32 architecture (32bit)
	# -c = Don't link (i.e., create an object file, not an executable)
	# -oaddresses.o = output compiled file to addresses.o
	

exec: addresses.c libs
	gcc -g -m32 -c -o addresses.o addresses.c
	gcc -g -m32  addresses.o -o addresses
	rm addresses.o

.PHONY: clean
clean:
	rm -rf ./*.o addresses
