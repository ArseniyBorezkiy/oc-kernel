#include <arch/reg.h>
#include <sched/task.h>
#include <sched/sched.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <lib/string.h>
#include <lib/time.h>
#include <messages.h>

int current_task_index = -1;

/*
 * Api - Schedule task to run
 */
extern void sched_schedule(size_t *ret_addr, size_t *reg_addr) {
  struct sched_task *this_task = null;
  struct sched_task *next_task = null;
  int next_task_index;

  next_task_index = sched_find_task_to_run_index(current_task_index);

  /* check task found */
  if (next_task_index == -1) {
    kpanic(MSG_SCHED_NO_TASKS);
  }

  /* load data */
  if (current_task_index != -1) {
    this_task = get_task_by_index(current_task_index);
  }
  next_task = get_task_by_index(next_task_index);

  if (this_task) {
    this_task->time += 1;

    /* check quota exceed */
    if (this_task->time < TASK_QUOTA) {
      return; /* continue current task execution */
    }

    /* reset quota */
    this_task->time = 0;

    /* save current task state */
    this_task->pc = *ret_addr;
    this_task->cs = *(u16*)((size_t)ret_addr + 4);
    this_task->flags = *(u32*)((size_t)ret_addr + 6);
    this_task->sp = (size_t)ret_addr + 10;
    memcpy(this_task->registers, (void*)reg_addr, sizeof(this_task->registers));
    this_task->registers[3] = this_task->sp;
  }

  kprint("scheduled tid=%u sp=%X pc=%X->%X\n", next_task_index, ret_addr, *ret_addr, next_task->pc);
  current_task_index = next_task_index;

  /* prepare context for the next task */
  *(u32*)(next_task->sp - 4) = next_task->flags;
  *(u16*)(next_task->sp - 6) = next_task->cs;
  *(u32*)(next_task->sp - 10) = next_task->pc;
  // stack_dump((size_t*)(next_task->sp - 10));
  next_task->sp = next_task->sp - 10;
  next_task->registers[3] = this_task->sp;
  next_task->sp = next_task->sp - sizeof(next_task->registers);
  memcpy((size_t*)next_task->sp, next_task->registers, sizeof(next_task->registers));
  
  // delay(5);

  /* switch context */
  asm_switch_context(next_task->sp);
}
