#
## Kernel make file
#

IDIR=./include
CC=gcc
AS=as
LD=ld
DD=dd
CC_FLAGS=-g -m32 -isystem $(IDIR) -I include -DKERNEL=1 -fno-stack-protector -Wall -Werror -fno-pie
AS_FLAGS=-g --32
LD_FLAGS=-m elf_i386

all: build start

#
# Build kernel
#
build: build-kernel build-lib
	$(LD) $(LD_FLAGS) -T ./config/link.ld -o ./bin/kernel.elf \
		./bin/entry.s.o ./bin/kernel.c.o ./bin/string.c.o

build-kernel: ./kernel/kernel.c
	$(CC) $(CC_FLAGS) -c ./kernel/kernel.c -o ./bin/kernel.c.o

build-kernel-arch: ./kernel/arch/entry.s
	$(AS) $(AS_FLAGS) ./kernel/arch/entry.s -o ./bin/entry.s.o

build-lib: ./lib/string.c
	$(CC) $(CC_FLAGS) -c ./lib/string.c -o ./bin/string.c.o

#
# Run kernel in emulator
#
start:
	qemu-system-i386 -kernel ./bin/kernel.elf
