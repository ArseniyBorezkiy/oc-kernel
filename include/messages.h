#pragma once

/* suffixes */
#define MSG_KERNEL_PREFIX "[KERNEL]: "
#define MSG_CRLN "\n"

/* kernel */
#define MSG_KERNEL_START (MSG_KERNEL_PREFIX "starting kernel at %X" MSG_CRLN)
#define MSG_KERNEL_MEM_AVAILABLE (MSG_KERNEL_PREFIX "total memory available %X Kb" MSG_CRLN)
#define MSG_KERNEL_STARTED (MSG_KERNEL_PREFIX "kernel started successfully" MSG_CRLN)
#define MSG_KERNEL_PANIC (MSG_KERNEL_PREFIX "kernel panic!" MSG_CRLN)
#define MSG_KERNEL_LINE (MSG_KERNEL_PREFIX "line: %u" MSG_CRLN)
#define MSG_IRQ1 (MSG_KERNEL_PREFIX "irq1" MSG_CRLN)

/* scheduler */
#define MSG_SCHED_TID_EXISTS (MSG_KERNEL_PREFIX "task id already exists" MSG_CRLN)
#define MSG_SCHED_TID_UNKNOWN (MSG_KERNEL_PREFIX "task id not found" MSG_CRLN)
#define MSG_SCHED_TID_EXCEED (MSG_KERNEL_PREFIX "tasks limit exceeded" MSG_CRLN)
#define MSG_SCHED_TID_CREATED (MSG_KERNEL_PREFIX "created task pc=%X" MSG_CRLN)
#define MSG_SCHED_NO_TASKS (MSG_KERNEL_PREFIX "there are no tasks to run" MSG_CRLN)
#define MSG_TASK_INIT_LAUNCHED (MSG_KERNEL_PREFIX "task init is launched sp=%X" MSG_CRLN)
#define MSG_TASK_TTY_LAUNCHED (MSG_KERNEL_PREFIX "task tty is launched sp=%X" MSG_CRLN)
