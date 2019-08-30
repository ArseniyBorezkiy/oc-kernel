#pragma once

#include <lib/stdtypes.h>

#define SYSCALL_KSEND 1
#define SYSCALL_KRECEIVE 2
#define SYSCALL_KILL 3
#define SYSCALL_EXIT 4
#define SYSCALL_READ 5
#define SYSCALL_WRITE 6
#define SYSCALL_OPEN 7
#define SYSCALL_CLOSE 8
#define SYSCALL_IOCTL 9

#define noreturn while(1) {} /* prevent gcc warning */

/* 
 * Api
 */
extern size_t asm_syscall(u_int function, ...);
