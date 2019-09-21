# Ars Boretskii OC

## UNIX like, multitask, i80386 PM + MMU, well-commented.

Well commented kernel based on micro architecture.
This helps you to write your own OC.
Main goal to give you small well-commended features that you can reuse in your OC.
Currently all tasks work in kernel mode.

### Content

- scheduler (/kernel/sched)
- initial ram disk (/initrd)
- shell task (/initrd/sh.c)
- tty character device driver (/kernel/dev/tty.c)
- abstract file operations (/kernel/vfs/file.c)
- interprocess communication (/kernel/ipc)
- architecture support (/arch)
- small c lib with lists support (/lib)

### Info

Target processor: i80386 (Protected mode).

### Dependencies

First, share folder on via VirtualBox.

```
$ VBoxManage sharedfolder add <vmname> --name <name> --hostpath <path>
```

```
On your guest os:
$ sudo apt-get install virtualbox-guest-utils
$ sudo mount -t vboxsf -o uid=1000,gid=1000 <name> <target-path>
```

When shared folder is ready, install build tools.

```
Linux lite (support qemu only):
$ sudo apt-get install gcc
$ sudo apt-get install gcc-arm-none-eabi
$ sudo apt-get install qemu
```
```
Ubuntu (support both bochs and qemu):
$ sudo apt-get install gcc
$ sudo apt-get install bochs
$ sudo apt-get install bochs-x
```

### Build sources and run on qemu

```
Qemu x86:
$ mkdir bin
$ sudo make qemu-x86
```

```
Qemu arm:
$ mkdir bin
$ sudo make qemu-arm
```

### Build sources and run on bochs

```
Bochs x86:
$ sudo mkdir bin
$ sudo mkdir /mnt/kernel_u
$ sudo losetup -a               # check loop13 is free
$ sudo losetup -d /dev/loop13   # release it if not free
$ sudo losetup -a               # make shure loop13 is not present
$ sudo make bochs-x86
```

### Shell commands

Process list:
```
Ars Boretskii kernel 1.0
# ps
```

Exit from current process (returns you to kernel console):
```
Ars Boretskii kernel 1.0
# exit
```

Kill process:
```
Ars Boretskii kernel 1.0
# kill 1
```

Exec elf on initrd:
```
Ars Boretskii kernel 1.0
# exec <name>
```

Clear screen:
```
Ars Boretskii kernel 1.0
# clear
```

Example:
```
Ars Boretskii kernel 1.0
# ps
# clear
# exec sh.elf
# ps
# kill 4
# exit
```

### Contributors
- Ilya Starchenko (thanks for bochs)

### Wish list

- User mode & TSS (ring3)
- Extend initrd to support more that 4Kb (reseach needed why qemu cut it)
- Forth interpreter (as next initrd elf)
