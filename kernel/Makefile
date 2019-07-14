all: build start

build: kernel.asm
	as kernel.s -o ./bin/ka.o
	gcc -c kernel.c -o ./bin/kc.o
	#nasm -f elf32 kernel.asm -o ./bin/ka.o
	#gcc -m32 -c kernel.c -o ./bin/kc.o
	ld -m elf_i386 -T ./config/link.ld -o ./bin/kernel.elf ./bin/ka.o ./bin/kc.o

start:
	qemu-system-i386 -kernel ./bin/kernel.elf
