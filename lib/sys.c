#include <lib/sys.h>
#include <lib/assert.h>
#include <lib/syscall.h>
#include <sched/task.h>

/*
 * Api - kill task
 */
extern bool kill(u_int tid) {
    return !!asm_syscall(SYSCALL_KILL, tid);
}

/*
 * Api - exit from current process
 */
extern void exit(int errno) {
    asm_syscall(SYSCALL_EXIT, errno);
    noreturn;
}

/*
 * Api - get tasks list
 */
extern struct clist_definition_t* ps() {
    return (void *)asm_syscall(SYSCALL_TASK_LIST);
}

/*
 * Api - get devices list
 */
extern struct clist_definition_t* devs() {
    return (void *)asm_syscall(SYSCALL_DEVS_LIST);
}

/*
 * Api - execute file
 */
 extern bool exec(char *name) {
     return !!asm_syscall(SYSCALL_EXEC, name);
 }
