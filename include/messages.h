#pragma once

/* suffixes */
#define MSG_KERNEL_PREFIX "[KERNEL]: "
#define MSG_SCHED_PREFIX "[SCHED]: "
#define MSG_TASK_PREFIX "[TASK]: "
#define MSG_INT_PREFIX "[INT]: "
#define MSG_CRLN "\n"

/* kernel */
#define MSG_KERNEL_NAME ("(c) Ars Boretskii Kernel 1.0" MSG_CRLN)
#define MSG_KERNEL_START (MSG_KERNEL_PREFIX "starting kernel at %X (memory available %X Kb)" MSG_CRLN)
#define MSG_KERNEL_STARTED (MSG_KERNEL_PREFIX "kernel started successfully" MSG_CRLN)
#define MSG_KERNEL_SCHEDULER_INITIALIZED (MSG_KERNEL_PREFIX "scheduler initialized" MSG_CRLN)
#define MSG_KERNEL_PANIC (MSG_KERNEL_PREFIX "kernel panic!" MSG_CRLN)
#define MSG_KERNEL_FILE_LINE (MSG_KERNEL_PREFIX "file: %s, line: %u" MSG_CRLN)
#define MSG_KERNEL_STACK_EXPECTED (MSG_KERNEL_PREFIX "kernel stack expected, but esp = %X" MSG_CRLN)
#define MSG_KERNEL_TASK_BUFF_EXCEED (MSG_KERNEL_PREFIX "message buffer exceed for tid = %X" MSG_CRLN)
#define MSG_KERNEL_CODE_UNREACHABLE (MSG_KERNEL_PREFIX "code unreachable" MSG_CRLN)
#define MSG_KERNEL_SYSLOG_INITIALIZED (MSG_KERNEL_PREFIX "syslog initialized" MSG_CRLN)
#define MSG_KERNEL_HEAP_EXCEED (MSG_KERNEL_PREFIX "kernel heap memory limit exceed" MSG_CRLN)
#define MSG_KERNEL_HEAP_TABLE_EXCEED (MSG_KERNEL_PREFIX "kernel heap memory table limit exceed" MSG_CRLN)
#define MSG_KERNEL_INITRD_AREA (MSG_KERNEL_PREFIX "init ram disk area %X-%X" MSG_CRLN)
#define MSG_KERNEL_ELF_LOADING (MSG_KERNEL_PREFIX "loading elf (sections=%u)" MSG_CRLN)
#define MSG_IRQ (MSG_KERNEL_PREFIX "irq %u" MSG_CRLN)
#define MSG_SYSCALL (MSG_KERNEL_PREFIX "syscall %u" MSG_CRLN)
#define MSG_INT_GP (MSG_INT_PREFIX "general protect" MSG_CRLN)
#define MSG_INT_PF (MSG_INT_PREFIX "page fault (%X)" MSG_CRLN)
#define MSG_INT_AC (MSG_INT_PREFIX "alignment check" MSG_CRLN)
#define MSG_INT_DF (MSG_INT_PREFIX "double fault" MSG_CRLN)
#define MSG_INT_IO (MSG_INT_PREFIX "invalid opcode" MSG_CRLN)
#define MSG_INT_DZ (MSG_INT_PREFIX "division by zero" MSG_CRLN)

/* scheduler */
#define MSG_SCHED_TID_EXISTS (MSG_SCHED_PREFIX "task id already exists" MSG_CRLN)
#define MSG_SCHED_TID_UNKNOWN (MSG_SCHED_PREFIX "task id not found" MSG_CRLN)
#define MSG_SCHED_TID_EXCEED (MSG_SCHED_PREFIX "tasks limit exceeded" MSG_CRLN)
#define MSG_SCHED_TID_CREATE (MSG_SCHED_PREFIX "create task tid = %u, eip = %X" MSG_CRLN)
#define MSG_SCHED_TID_DELETE (MSG_SCHED_PREFIX "delete task tid = %u" MSG_CRLN)
#define MSG_SCHED_NO_TASKS (MSG_SCHED_PREFIX "there are no tasks to run" MSG_CRLN)
#define MSG_SCHED_NEXT (MSG_SCHED_PREFIX "scheduled tid=%u sp=%X eip=%X->%X" MSG_CRLN)
#define MSG_TASK_INIT_LAUNCHED (MSG_TASK_PREFIX "task init is launched esp = %X, eflags = %X" MSG_CRLN)
