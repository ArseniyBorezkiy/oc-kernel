all: boot copy info run
	@echo ready

clean: disk.bin
	@rm -f ./disk.bin

boot: boot.s loader.c
	as -o boot.o boot.s
	cc -o loader.o -c loader.c
	ld -Ttext 0x7C00 --oformat binary -arch i386 -o boot.bin boot.o
	ld -Ttext 0x0000 --oformat binary -arch i386 -o loader.bin loader.o

copy: boot.bin
	dd if=/dev/zero of=disk.bin bs=1024 count=1440
	dd if=boot.bin of=disk.bin conv=notrunc
	# dd seek=600 bs=1 if=loader.bin of=disk.bin conv=notrunc

info: disk.bin
	@objdump -m i386 -b binary -D disk.bin

run: disk.bin
	@qemu-system-i386 -fda disk.bin -boot a -monitor stdio
