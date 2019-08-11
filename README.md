# Ars Boretskii OC
## UNIX like, multitask, i80386 PM + MMU, well-commented.

Well commented kernel based on micro architecture.
This helps you to write your own OC.
Main goal to give you small well-commended features that you can reuse in your OC.

### Content
- scheduler (/kernel/sched)
- shell task (/kernel/tasks/sh.c)
- tty task (/kernel/tasks/tty.c)
- init task stub (/kernel/tasks/init.c)
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
Kernel syslog view:
```
Ars Boretskii kernel 1.0
# syslog
```
