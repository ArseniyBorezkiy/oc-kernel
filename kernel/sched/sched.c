#include <arch/reg.h>
#include <arch/idt.h>
#include <sched/task.h>
#include <sched/sched.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <utils/kassert.h>
#include <lib/string.h>
#include <lib/time.h>
#include <lib/stdtypes.h>
#include <messages.h>

int current_task_index = -1; /* current running process index */

/*
 * Api - Init
 */
extern void sched_init() {
  task_init();
  /* no task currently running */
  current_task_index = -1;
}

/*
 * Api - Schedule task to run
 */
extern void sched_schedule(size_t *ret_addr, size_t *reg_addr) {
  struct sched_task *this_task = null;
  struct sched_task *next_task = null;
  int next_task_index;

  /* get current task */
  if (current_task_index != -1) {
    this_task = task_get_by_index(current_task_index);
    kassert(__FILE__, __LINE__, this_task != null);

    /* update running time */
    this_task->time += 1;

    /* check quota exceed */
    if (this_task->time < TASK_QUOTA && !this_task->reschedule) {
      return; /* continue task execution */
    }

    /* reset quota */
    this_task->time = 0;
    this_task->reschedule = false;

    /* save task state */
    this_task->op_registers.eip = *ret_addr;
    this_task->op_registers.cs = *(u16*)((size_t)ret_addr + 4);
    *(u32*)(&this_task->flags) = *(u32*)((size_t)ret_addr + 6);
    this_task->op_registers.esp = (size_t)ret_addr + 10;
    this_task->gp_registers.esp = this_task->op_registers.esp;
    memcpy(&this_task->gp_registers, (void*)reg_addr, sizeof(this_task->gp_registers));
  }

  /* pick next task */
  next_task_index = task_find_to_run_index(current_task_index);
  if (next_task_index == -1) {
    kpanic(MSG_SCHED_NO_TASKS);
  }
  next_task = task_get_by_index(next_task_index);

  /* update current task pointer */
  kprint("scheduled tid=%u sp=%X pc=%X->%X\n", next_task_index, ret_addr, *ret_addr, next_task->op_registers.eip);
  current_task_index = next_task_index;

  /* prepare context for the next task */
  *(u32*)(next_task->op_registers.esp - 4) = *(u32*)(&next_task->flags);
  *(u16*)(next_task->op_registers.esp - 6) = next_task->op_registers.cs;
  *(u32*)(next_task->op_registers.esp - 10) = next_task->op_registers.eip;
  // stack_dump((size_t*)(next_task->sp - 10));
  next_task->op_registers.esp = next_task->op_registers.esp - 10;
  next_task->gp_registers.esp = this_task->op_registers.esp;
  next_task->op_registers.esp = next_task->op_registers.esp - sizeof(next_task->gp_registers);
  memcpy((void*)next_task->op_registers.esp, &next_task->gp_registers, sizeof(next_task->gp_registers));
  
  // delay(5);

  /* switch context */
  asm_switch_context(next_task->op_registers.esp);
}

/*
 * Api - Get current running task
 */
extern struct sched_task *sched_get_current_task() {
  struct sched_task *task;
  task = task_get_by_index(current_task_index);
  kassert(__FILE__, __LINE__, task != null);
  return task;
}

/*
 * Api - Yield current process
 */
extern void sched_yield() {
  struct sched_task *task;
  /* get current task */
  task = sched_get_current_task();
  /* mark task to be rescheduled */
  task->reschedule = true;
  /* reschedule */
  asm_int(INT_TIMER);
}
