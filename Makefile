#
# Kernel make file
#

all: build start

#
# Build kernel
#
build: build-utils build-lib build-arch ./kernel/kernel.s ./kernel/kernel.c
	as -g --32 ./kernel/kernel.s -o ./bin/ka.o
	gcc -g -m32 -isystem ./include -I include -c ./kernel/kernel.c -o ./bin/kc.o
	ld -m elf_i386 -T ./config/link.ld -o ./bin/kernel.elf \
		./bin/ka.o ./bin/kc.o ./bin/kprint.o \
		./bin/math.o ./bin/string.o \
		./bin/port.s.o ./bin/interrupt.s.o ./bin/interrupt.c.o

build-utils:
	gcc -g -m32 -isystem ./include -I include -c ./kernel/utils/kprint.c -o ./bin/kprint.o

build-lib:
	gcc -g -m32 -isystem ./include -I include -c ./lib/string.c -o ./bin/string.o
	gcc -g -m32 -isystem ./include -I include -c ./lib/math.c -o ./bin/math.o

build-arch:
	as -g --32 ./kernel/arch/port.s -o ./bin/port.s.o
	as -g --32 ./kernel/arch/interrupt.s -o ./bin/interrupt.s.o
	gcc -g -m32 -isystem ./include -I include -fno-stack-protector -c ./kernel/arch/interrupt.c -o ./bin/interrupt.c.o

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
	gcc -Wa,-adhln -m32 -ggdb -isystem ./include -I include -c ./kernel/kernel.c -o ./bin/kc.o > ./bin/kernel.c.txt
	gcc -Wa,-adhln -m32 -ggdb -isystem ./include -I include -c ./kernel/arch/interrupt.c -o ./bin/interrupt.c.o > ./bin/interrupt.c.txt
	gcc -Wa,-adhln -m32 -ggdb -isystem ./include -I include -c ./kernel/utils/kprint.c -o ./bin/kprint.o > ./bin/kprint.c.txt

