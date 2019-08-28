# Ars Boretskii OC

## UNIX like, multitask, i80386 PM + MMU, well-commented.

Well commented kernel based on micro architecture.
This helps you to write your own OC.
Main goal to give you small well-commended features that you can reuse in your OC.
Currently all tasks work in kernel mode.

This OC will evolute according to wish list section.

### Content

- scheduler (/kernel/sched)
- initial ram disk (/initrd)
- shell task (/initrd/sh.c)
- tty device driver (/kernel/dev/tty.c)
- interprocess communication (/kernel/ipc)
- interprocess synchronization (/kernel/sync)
- architecture support (/arch)
- list implementations (/lib/data)
- small c lib (/lib)

### Info

Target processor: i80386 (Protected mode).

### Dependencies

```
$ apt-get install gcc
$ apt-get install qemu
```

### Build and run

```
$ sudo make
```

### Shell commands

Task list:

```
Ars Boretskii kernel 1.0
# ps
```

Kill task:

```
Ars Boretskii kernel 1.0
# kill 1
```

Kernel syslog view:

```
Ars Boretskii kernel 1.0
# syslog
```

### Wish list

- Ram disk
- Memory protect
- User mode (ring3)
- System calls
- Some POSIX functions for c lib
