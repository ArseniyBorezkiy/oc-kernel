#include <boot/multiboot.h>
#include <arch/idt.h>
#include <arch/pic.h>
#include <sched/task.h>
#include <tasks/tty.h>
#include <tasks/init.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <utils/lib.h>
#include <lib/time.h>
#include <lib/stdtypes.h>
#include <messages.h>
#include <kernel.h>

extern void *kernel_stack = null;

static void kernel_create_tasks();
static void kernel_run_tasks();

/*
 * Api - Kernel entry point
 */
extern void kernel_start(struct multiboot_t *multiboot, void *kstack)
{
  kclear();
  kprint(MSG_KERNEL_START, &kernel_start);
  kprint(MSG_KERNEL_MEM_AVAILABLE, multiboot->mem_upper);

  /* remember kernel stack addr */
  kernel_stack = kstack;
  
  /* init arch */
  idt_init();
  lib_init();
  mmu_init();

	/* init scheduler */
  sched_init();
  kernel_create_tasks();
  kernel_run_tasks();

  /* enable interrupts */
  pic_enable();
  asm_unlock();

  kprint(MSG_KERNEL_STARTED);

  /* should never return */
	while(1) {
    delay(1);
    kprint(MSG_KERNEL_STARTED);
  }
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
