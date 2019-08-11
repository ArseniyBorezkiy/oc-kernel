#pragma once

/* suffixes */
#define MSG_KERNEL_PREFIX "[KERNEL]: "
#define MSG_SCHED_PREFIX "[SCHED]: "
#define MSG_TASK_PREFIX "[TASK]: "
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
#define MSG_IRQ1 (MSG_KERNEL_PREFIX "irq1" MSG_CRLN)

/* scheduler */
#define MSG_SCHED_TID_EXISTS (MSG_SCHED_PREFIX "task id already exists" MSG_CRLN)
#define MSG_SCHED_TID_UNKNOWN (MSG_SCHED_PREFIX "task id not found" MSG_CRLN)
#define MSG_SCHED_TID_EXCEED (MSG_SCHED_PREFIX "tasks limit exceeded" MSG_CRLN)
#define MSG_SCHED_TID_CREATE (MSG_SCHED_PREFIX "create task tid = %u, eip = %X" MSG_CRLN)
#define MSG_SCHED_TID_DELETE (MSG_SCHED_PREFIX "delete task tid = %u" MSG_CRLN)
#define MSG_SCHED_NO_TASKS (MSG_SCHED_PREFIX "there are no tasks to run" MSG_CRLN)
#define MSG_SCHED_NEXT (MSG_SCHED_PREFIX "scheduled tid=%u sp=%X cs=%X eip=%X->%X" MSG_CRLN)
#define MSG_TASK_INIT_LAUNCHED (MSG_TASK_PREFIX "task init is launched esp = %X, eflags = %X" MSG_CRLN)
#define MSG_TASK_TTY_LAUNCHED (MSG_TASK_PREFIX "task tty is launched esp = %X" MSG_CRLN)
