all: prog.out prog-cross.out

prog.out: src/prog.c
	gcc -o prog.out src/prog.c

prog-cross.out: src/prog.c
	arm-linux-gnueabi-gcc -o prog-cross.out src/prog.c

clean:
	rm -f prog.out prog-cross.out
