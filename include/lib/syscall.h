#pragma once

#include <lib/stdtypes.h>

#define SYSCALL_KSEND 1
#define SYSCALL_KRECEIVE 2
#define SYSCALL_KILL 3
#define SYSCALL_EXIT 4
#define SYSCALL_EXEC 5
#define SYSCALL_READ 6
#define SYSCALL_WRITE 7
#define SYSCALL_OPEN 8
#define SYSCALL_CLOSE 9
#define SYSCALL_IOCTL 10
#define SYSCALL_TASK_LIST 11
#define SYSCALL_DEVS_LIST 12

#define noreturn while(1) {} /* prevent gcc warning */

/* 
 * Api
 */
extern size_t asm_syscall(u_int function, ...);
