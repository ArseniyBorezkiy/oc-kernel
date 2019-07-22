#include <messages.h>
#include <arch/memory.h>
#include <arch/idt.h>
#include <arch/pic.h>
#include <sched/task.h>
#include <tasks/tty.h>
#include <tasks/init.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>

static void kernel_create_tasks();
static void kernel_run_tasks();

/*
 * Api - Kernel entry point
 */
extern void kernel_start(void)
{
  kclear();
  kprint(MSG_KERNEL_START, (size_t)&kernel_start);
  
  /* init arch */
  idt_init();

	/* init scheduler */
  kernel_create_tasks();
  kernel_run_tasks();

  /* enable interrupts */
  pic_enable();
  asm_unlock();

  kprint(MSG_KERNEL_STARTED);

  /* should never return */
	while(1);
}

/*
 * Create kernel tasks
 */
static void kernel_create_tasks() {
  bool is_ok = 
    sched_create_task(TID_INIT, task_init) &&
    sched_create_task(TID_TTY, task_tty);

  if (!is_ok) {
    kpanic(MSG_KERNEL_PANIC);
  }
}

/*
 * Run kernel tasks
 */
static void kernel_run_tasks() {
  bool is_ok = 
    sched_run_task_by_id(TID_TTY) &&
    sched_run_task_by_id(TID_INIT);

  if (!is_ok) {
    kpanic(MSG_KERNEL_PANIC);
  }
}
