#include <arch/arch.h>
#include <lib/assert.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/stdtypes.h>
#include <lib/string.h>
#include <lib/time.h>
#include <messages.h>
#include <sched/sched.h>
#include <sched/task.h>
#include <utils/kheap.h>
#include <kernel.h>

static struct task_t* current_task; /* current running process */

/*
 * Api - Init 
 */
extern void sched_init()
{
    /* no task currently running */
    current_task = null;
    /* init task module */
    task_init();

    printf(MSG_KERNEL_SCHEDULER_INITIALIZED);
}

/*
 * Api - Schedule task to run
 */
extern void sched_schedule(size_t* ret_addr, size_t* reg_addr)
{
    struct task_t* next_task = null;

    /* finish current task */
    if (current_task != null) {
        /* update running time */
        current_task->time += 1;

        /* check quota exceed */
        if (current_task->time < TASK_QUOTA && !current_task->reschedule) {
            return; /* continue task execution */
        }

        /* reset quota */
        current_task->time = 0;
        current_task->reschedule = false;

        /* save task state */
        current_task->op_registers.eip = *ret_addr;
        current_task->op_registers.cs = *(u16*)((size_t)ret_addr + 4);
        *(u32*)(&current_task->flags) = *(u32*)((size_t)ret_addr + 6) | 0x200;
        current_task->op_registers.u_esp = (size_t)ret_addr + 12;
        current_task->gp_registers.esp = current_task->op_registers.u_esp;
        memcpy(&current_task->gp_registers, (void*)reg_addr,
            sizeof(struct gp_registers_t));
    }

    /* pick next task */
    if (current_task) {
        next_task = task_get_next_by_status(TASK_RUNNING, current_task);
    } else {
        next_task = task_get_by_status(TASK_RUNNING);
        tss_set_kernel_stack(next_task->kstack);
    }
    assert(next_task != null);

    /* whether should kill current task */
    if (current_task && current_task->status == TASK_KILLING) {
        /* kill current task */
        task_delete(current_task);
    } else {
        /* try to kill killing tasks */
        struct task_t* task;
        task = task_find_by_status(TASK_KILLING);
        if (task) {
            task_delete(task);
        }
    }

    /* prepare context for the next task */
    next_task->op_registers.u_esp -= 4;
    *(u32*)(next_task->op_registers.u_esp) = (*(u16*)(&next_task->flags)) | 0x200;
    next_task->op_registers.u_esp -= 4;
    *(u32*)(next_task->op_registers.u_esp) = next_task->op_registers.cs;
    next_task->op_registers.u_esp -= 4;
    *(u32*)(next_task->op_registers.u_esp) = next_task->op_registers.eip;
    next_task->gp_registers.esp = next_task->op_registers.u_esp;
    next_task->op_registers.u_esp -= sizeof(struct gp_registers_t);
    memcpy((void*)next_task->op_registers.u_esp,
        (void*)&next_task->gp_registers, sizeof(struct gp_registers_t));

    /* update current task pointer */
    current_task = next_task;

    /* run next task */
    printf(MSG_SCHED_NEXT, next_task->tid, next_task->op_registers.u_esp,
        *ret_addr, next_task->op_registers.eip);
    asm_switch_ucontext(next_task->op_registers.u_esp,
        next_task->op_registers.cr3);
}

/*
 * Api - Get current running task
 */
extern struct task_t* sched_get_current_task()
{
    assert(current_task != null);
    return current_task;
}

/*
 * Api - Yield current process
 */
extern void sched_yield()
{
    /* reschedule current task */
    if (current_task) {
        current_task->reschedule = true;
    }
    /* launch scheduler */
    __asm__("int $0x20");
}
