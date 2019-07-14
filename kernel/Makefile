#
# Kernel make file
#

all: build start

#
# Build kernel
#
build: build-utils kernel.s kernel.c
	as -g --32 kernel.s -o ./bin/ka.o
	gcc -g -m32 -isystem ./include -I include -c kernel.c -o ./bin/kc.o
	ld -m elf_i386 -T ./config/link.ld -o ./bin/kernel.elf ./bin/ka.o ./bin/kc.o ./bin/kprint.o

build-utils:
	gcc -g -m32 -isystem ./include -I include -c ./utils/kprint.c -o ./bin/kprint.o

#
# Run kernel in emulator
#
start:
	qemu-system-i386 -d guest_errors -kernel ./bin/kernel.elf

#
# Delete binary files
#
clean:
	rm -f ./bin/*

#
# Debug kernel
#
debug:
	gdb -x ./debug.script -s ./bin/kernel.elf

#
# Listing
#
list:
	gcc -Wa,-adhln -m32 -ggdb -isystem ./include -I include -c kernel.c -o ./bin/kc.o > ./bin/kernel.c.txt

