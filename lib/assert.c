#include <lib/assert.h>
#include <lib/stdtypes.h>
#include <lib/syscall.h>

/*
 * Api - assertion
 */
extern void uassert(const char* file, u_int line, bool expr)
{
    asm_syscall(SYSCALL_KILL);
    noreturn;
}

/* 
 * Api - code unreachable
 */
extern void uunreachable(const char* file, u_int line)
{
    asm_syscall(SYSCALL_KILL);
    noreturn;
}
