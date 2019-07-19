#
## Kernel make file
#

IDIR=./include
CC=gcc
AS=as
LD=ld
CC_FLAGS=-g -m32 -isystem $(IDIR) -I include -fno-stack-protector -Wall -Werror
AS_FLAGS=-g --32
LD_FLAGS=-m elf_i386 -T ./config/link.ld

all: build start

#
# Build kernel
#
build: build-lib build-kernel
	$(LD) $(LD_FLAGS) -o ./bin/kernel.elf \
		./bin/kernel.s.o ./bin/kernel.c.o ./bin/kprint.c.o \
		./bin/math.c.o ./bin/string.c.o \
		./bin/port.s.o ./bin/idt.s.o ./bin/idt.c.o ./bin/pic.c.o \
		./bin/task.s.o ./bin/task.c.o \
		./bin/init.c.o ./bin/tty.c.o

build-lib: ./lib/string.c ./lib/math.c
	$(CC) $(CC_FLAGS) -c ./lib/string.c -o ./bin/string.c.o
	$(CC) $(CC_FLAGS) -c ./lib/math.c -o ./bin/math.c.o

build-kernel: build-kernel-utils build-kernel-arch build-kernel-sched build-kernel-tasks \
              ./kernel/kernel.s ./kernel/kernel.c
	$(AS) $(AS_FLAGS) ./kernel/kernel.s -o ./bin/kernel.s.o
	$(CC) $(CC_FLAGS) -c ./kernel/kernel.c -o ./bin/kernel.c.o

build-kernel-utils: ./kernel/utils/kprint.c
	$(CC) $(CC_FLAGS) -c ./kernel/utils/kprint.c -o ./bin/kprint.c.o

build-kernel-arch: ./kernel/arch/port.s ./kernel/arch/idt.s ./kernel/arch/pic.c ./kernel/arch/idt.c
	$(AS) $(AS_FLAGS) ./kernel/arch/port.s -o ./bin/port.s.o
	$(AS) $(AS_FLAGS) ./kernel/arch/idt.s -o ./bin/idt.s.o
	$(CC) $(CC_FLAGS) -c ./kernel/arch/pic.c -o ./bin/pic.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/arch/idt.c -o ./bin/idt.c.o

build-kernel-sched: ./kernel/sched/task.s ./kernel/sched/task.c
	$(AS) $(AS_FLAGS) ./kernel/sched/task.s -o ./bin/task.s.o
	$(CC) $(CC_FLAGS) -c ./kernel/sched/task.c -o ./bin/task.c.o

build-kernel-tasks: ./kernel/tasks/init.c ./kernel/tasks/tty.c
	$(CC) $(CC_FLAGS) -c ./kernel/tasks/init.c -o ./bin/init.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/tasks/tty.c -o ./bin/tty.c.o

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
	$(CC) $(CC_FLAGS) -Wa,-adhln -ggdb -c ./kernel/kernel.c -o ./bin/kernel.c.o > ./bin/kernel.c.txt
