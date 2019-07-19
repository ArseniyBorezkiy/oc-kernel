#pragma once

#define MSG_KERNEL_PREFIX "[KERNEL]: "
#define MSG_CRLN "\n"
#define MSG_KERNEL_START (MSG_KERNEL_PREFIX "starting kernel" MSG_CRLN)
#define MSG_IRQ1 (MSG_KERNEL_PREFIX "irq1" MSG_CRLN)

#define MSG_SCHED_TID_EXISTS (MSG_KERNEL_PREFIX "task id already exists" MSG_CRLN)
#define MSG_SCHED_TID_UNKNOWN (MSG_KERNEL_PREFIX "task id not found" MSG_CRLN)
#define MSG_SCHED_TID_EXCEED (MSG_KERNEL_PREFIX "tasks limit exceeded" MSG_CRLN)

#define MSG_TASK_INIT_LAUNCHED (MSG_KERNEL_PREFIX "task init is launched" MSG_CRLN)
#define MSG_TASK_TTY_LAUNCHED (MSG_KERNEL_PREFIX "task tty is launched" MSG_CRLN)
