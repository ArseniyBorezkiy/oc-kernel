#pragma once

#include <lib/stdtypes.h>

#define SYSCALL_KSEND 1
#define SYSCALL_KRECEIVE 2
#define SYSCALL_KILL 3
#define SYSCALL_WRITE 4
#define SYSCALL_IOCTL 5

/*
 * Api
 */
extern void asm_syscall(u_int function, ...);
