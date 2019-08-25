#include <lib/assert.h>
#include <lib/stdtypes.h>
#include <lib/syscall.h>

/*
 * Api - assertion
 */
extern void uassert(char *file, u_int line, bool expr) {
  asm_syscall(SYSCALL_KILL);
}

/*
 * Api - code unreachable
 */
extern void uunreachable(char *file, u_int line) {
  asm_syscall(SYSCALL_KILL);
}
