#pragma once

#include <lib/stdtypes.h>

#define SYSCALL_KSEND 1
#define SYSCALL_KRECEIVE 2
#define SYSCALL_KILL 3

/*
 * Api
 */
extern void asm_syscall(u_int function, ...);
