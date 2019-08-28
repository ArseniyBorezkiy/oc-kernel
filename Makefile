#
## Kernel make file
#

IDIR=./include
CC=gcc
AS=as
LD=ld
DD=dd
CC_FLAGS=-g -m32 -isystem $(IDIR) -I include -DKERNEL=1 -fno-stack-protector -Wall -Werror -fno-pie
CC_USER_FLAGS=-g -m32 -isystem $(IDIR) -I include -fno-stack-protector -Wall -Werror -fno-pie
AS_FLAGS=-g --32
LD_FLAGS=-m elf_i386
LD_USER_FLAGS=-m elf_i386

all: build start

#
# Build kernel
#
build: build-lib build-kernel build-initrd
	$(LD) $(LD_FLAGS) -T ./config/link.ld -o ./bin/kernel.elf \
		./bin/entry.s.o ./bin/kernel.c.o \
		./bin/kprint.c.o ./bin/kdump.c.o ./bin/kpanic.c.o ./bin/kheap.c.o ./bin/kassert.c.o \
		./bin/lib.c.o \
		./bin/assert.c.o ./bin/time.c.o ./bin/math.c.o ./bin/string.c.o ./bin/stdio.c.o ./bin/syscall.s.o \
		./bin/reg.s.o ./bin/port.s.o ./bin/idt.s.o ./bin/mmu.s.o \
		./bin/idt.c.o ./bin/ih.c.o ./bin/pic.c.o ./bin/mmu.c.o \
		./bin/task.c.o ./bin/sched.c.o \
		./bin/ipc.c.o \
		./bin/spin.c.o \
		./bin/video.c.o \
		./bin/slist.c.o ./bin/clist.c.o \
		./bin/mm.c.o \
		./bin/initrd.c.o ./bin/elf.c.o ./bin/file.c.o \
		./bin/init.c.o ./bin/tty.c.o ./bin/dev.c.o ./bin/sh.c.o

build-lib: ./lib/assert.c ./lib/time.c ./lib/string.c ./lib/math.c ./lib/stdio.c \
           ./lib/data/slist.c ./lib/data/clist.c
	$(CC) $(CC_FLAGS) -c ./lib/assert.c -o ./bin/assert.c.o
	$(CC) $(CC_FLAGS) -c ./lib/time.c -o ./bin/time.c.o
	$(CC) $(CC_FLAGS) -c ./lib/string.c -o ./bin/string.c.o
	$(CC) $(CC_FLAGS) -c ./lib/math.c -o ./bin/math.c.o
	$(CC) $(CC_FLAGS) -c ./lib/stdio.c -o ./bin/stdio.c.o
	$(AS) $(AS_FLAGS) ./lib/syscall.s -o ./bin/syscall.s.o
	$(CC) $(CC_FLAGS) -c ./lib/data/slist.c -o ./bin/slist.c.o
	$(CC) $(CC_FLAGS) -c ./lib/data/clist.c -o ./bin/clist.c.o

build-kernel: build-kernel-utils build-kernel-arch build-kernel-sched build-kernel-tasks \
              build-kernel-ipc build-kernel-sync build-kernel-dev build-kernel-vfs build-kernel-mm \
              ./kernel/kernel.c
	$(CC) $(CC_FLAGS) -c ./kernel/kernel.c -o ./bin/kernel.c.o

build-kernel-utils: ./kernel/utils/kprint.c ./kernel/utils/kdump.c ./kernel/utils/kpanic.c ./kernel/utils/kheap.c ./kernel/utils/kassert.c
	$(CC) $(CC_FLAGS) -c ./kernel/utils/kprint.c -o ./bin/kprint.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/utils/kdump.c -o ./bin/kdump.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/utils/kpanic.c -o ./bin/kpanic.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/utils/kheap.c -o ./bin/kheap.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/utils/kassert.c -o ./bin/kassert.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/utils/lib.c -o ./bin/lib.c.o

build-kernel-dev: ./kernel/dev/utils/video.c ./kernel/dev/tty.c ./kernel/dev/dev.c
	$(CC) $(CC_FLAGS) -c ./kernel/dev/utils/video.c -o ./bin/video.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/dev/tty.c -o ./bin/tty.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/dev/dev.c -o ./bin/dev.c.o

build-kernel-vfs: ./kernel/vfs/initrd.c ./kernel/vfs/elf.c ./kernel/vfs/file.c
	$(CC) $(CC_FLAGS) -c ./kernel/vfs/initrd.c -o ./bin/initrd.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/vfs/elf.c -o ./bin/elf.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/vfs/file.c -o ./bin/file.c.o

build-kernel-mm: ./kernel/mm/mm.c
	$(CC) $(CC_FLAGS) -c ./kernel/mm/mm.c -o ./bin/mm.c.o

build-kernel-arch: ./kernel/arch/reg.s ./kernel/arch/port.s ./kernel/arch/idt.s \
                   ./kernel/arch/pic.c ./kernel/arch/idt.c ./kernel/arch/mmu.s \
                   ./kernel/arch/mmu.c
	$(AS) $(AS_FLAGS) ./kernel/arch/entry.s -o ./bin/entry.s.o
	$(AS) $(AS_FLAGS) ./kernel/arch/reg.s -o ./bin/reg.s.o
	$(AS) $(AS_FLAGS) ./kernel/arch/port.s -o ./bin/port.s.o
	$(AS) $(AS_FLAGS) ./kernel/arch/idt.s -o ./bin/idt.s.o
	$(AS) $(AS_FLAGS) ./kernel/arch/mmu.s -o ./bin/mmu.s.o
	$(CC) $(CC_FLAGS) -c ./kernel/arch/pic.c -o ./bin/pic.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/arch/idt.c -o ./bin/idt.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/arch/ih.c -o ./bin/ih.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/arch/mmu.c -o ./bin/mmu.c.o

build-kernel-ipc: ./kernel/ipc/ipc.c
	$(CC) $(CC_FLAGS) -c ./kernel/ipc/ipc.c -o ./bin/ipc.c.o

build-kernel-sync: ./kernel/sync/spin.c
	$(CC) $(CC_FLAGS) -c ./kernel/sync/spin.c -o ./bin/spin.c.o

build-kernel-sched: ./kernel/sched/task.c ./kernel/sched/sched.c
	$(CC) $(CC_FLAGS) -c ./kernel/sched/task.c -o ./bin/task.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/sched/sched.c -o ./bin/sched.c.o

build-kernel-tasks: ./kernel/tasks/init.c ./kernel/tasks/sh.c
	$(CC) $(CC_FLAGS) -c ./kernel/tasks/init.c -o ./bin/init.c.o
	$(CC) $(CC_FLAGS) -c ./kernel/tasks/sh.c -o ./bin/sh.c.o

build-initrd: build-initrd-elfs ./bin/sh.elf
	rm ./bin/initrd.img
	touch ./bin/initrd.img
	$(DD) if=./bin/sh.elf of=./bin/initrd.img

build-initrd-elfs: ./initrd/sh.c
	$(CC) $(CC_USER_FLAGS) -c ./initrd/sh.c -o ./bin/sh.rd.c.o
	$(LD) $(LD_USER_FLAGS) -T ./config/link-task.ld -o ./bin/sh.elf ./bin/sh.rd.c.o \
		./bin/assert.c.o ./bin/time.c.o ./bin/math.c.o ./bin/string.c.o ./bin/stdio.c.o ./bin/syscall.s.o

#
# Run kernel in emulator
#   use -d int to debug mmu & faults & interrupts
#
start:
	qemu-system-i386 -no-reboot -no-shutdown -kernel ./bin/kernel.elf -initrd ./bin/initrd.img

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
# Dup kernel
#
dump:
	objdump -d ./bin/kernel.elf > ./bin/kernel.elf.dump.txt
	objdump -d ./bin/hello.elf > ./bin/hello.elf.dump.txt

headers:
	objdump -h ./bin/kernel.elf > ./bin/kernel.elf.head.txt
	objdump -x ./bin/hello.elf > ./bin/hello.elf.head.txt

#
# Listing
#
list:
	$(CC) $(CC_FLAGS) -Wa,-adhln -ggdb -c ./kernel/kernel.c -o ./bin/kernel.c.o > ./bin/kernel.c.txt
