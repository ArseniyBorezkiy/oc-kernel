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
build: build-kernel build-kernel-arch build-lib
	$(LD) $(LD_FLAGS) -T ./config/link.ld -o ./bin/kernel.elf \
		./bin/kernel.c.o \
		./bin/entry.s.o ./bin/port.s.o ./bin/reg.s.o ./bin/string.c.o \
		./bin/kassert.c.o ./bin/kprint.c.o ./bin/kpanic.c.o ./bin/video.c.o

build-kernel: ./kernel/kernel.c
	$(CC) $(CC_FLAGS) -c ./kernel/kernel.c -o ./bin/kernel.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/utils/kassert.c -o ./bin/kassert.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/utils/kprint.c -o ./bin/kprint.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/utils/kpanic.c -o ./bin/kpanic.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/dev/utils/video.c -o ./bin/video.c.o

build-kernel-arch: ./kernel/arch/entry.s
	$(AS) $(AS_FLAGS) ./kernel/arch/entry.s -o ./bin/entry.s.o
	$(AS) $(AS_FLAGS) ./kernel/arch/port.s -o ./bin/port.s.o
	$(AS) $(AS_FLAGS) ./kernel/arch/reg.s -o ./bin/reg.s.o

build-lib: ./lib/string.c
	$(CC) $(CC_FLAGS) -c ./lib/string.c -o ./bin/string.c.o

#
# Run kernel in emulator
#
start:
	qemu-system-i386 -kernel ./bin/kernel.elf
