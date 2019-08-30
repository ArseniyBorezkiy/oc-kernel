#include <lib/sys.h>
#include <lib/assert.h>
#include <lib/syscall.h>
#include <sched/task.h>

/*
 * Api - kill task
 */
extern void kill(u_int tid) {
    asm_syscall(SYSCALL_KILL, tid);
}

/*
 * Api - exit from current process
 */
extern void exit(int errno) {
    asm_syscall(SYSCALL_EXIT, errno);
    noreturn;
}

/*
 * Api - get current task
 */
extern struct clist_definition_t* ps() {
    return (void *)asm_syscall(SYSCALL_TASK_LIST);
}
