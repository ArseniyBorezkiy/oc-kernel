#include <boot/multiboot.h>
#include <arch/idt.h>
#include <arch/pic.h>
#include <arch/mmu.h>
#include <lib/data/slist.h>
#include <lib/data/clist.h>
#include <dev/video.h>
#include <sched/task.h>
#include <sched/sched.h>
#include <tasks/tty.h>
#include <tasks/init.h>
#include <tasks/sh.h>
#include <utils/kdump.h>
#include <utils/kheap.h>
#include <utils/lib.h>
#include <lib/time.h>
#include <lib/stdtypes.h>
#include <lib/assert.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <messages.h>
#include <kernel.h>

void *kernel_stack = null;

static void kernel_create_tasks();
static void kernel_run_tasks();

/*
 * Api - Kernel entry point
 */
extern void kernel_start(struct multiboot_t *multiboot, void *kstack)
{
  /* remember kernel stack addr */
  kernel_stack = kstack;

  /* init screen */
  video_init();
  clear();

  /* hello */
  printf(MSG_KERNEL_NAME);
  printf(MSG_KERNEL_START, &kernel_start, multiboot->mem_upper);

  /* init arch */
  lib_init();
  idt_init();
  mmu_init();

  /* init data structures */
  slist_init();
  clist_init();

  /* init dynamic memory */
  kheap_init();
  
  /* init scheduler */
  sched_init();
  printf(MSG_KERNEL_SCHEDULER_INITIALIZED);
  kernel_create_tasks();
  kernel_run_tasks();

  /* enable interrupts */
  pic_enable();
  asm_unlock();

  /* start scheduler */
  printf(MSG_KERNEL_STARTED);
  sched_yield();
  unreachable();
}

/*
 * Create kernel tasks
 */
static void kernel_create_tasks()
{
  bool is_ok =
      task_create(TID_INIT, task_init_main) &&
      task_create(TID_TTY, task_tty_main) &&
      task_create(TID_SH, task_sh_main);
  assert(is_ok);
}

/*
 * Run kernel tasks
 */
static void kernel_run_tasks()
{
  struct task_t *init_task;
  struct task_t *tty_task;
  struct task_t *sh_task;

  init_task = task_get_by_id(TID_INIT);
  tty_task = task_get_by_id(TID_TTY);
  sh_task = task_get_by_id(TID_SH);

  init_task->status = TASK_RUNNING;
  tty_task->status = TASK_RUNNING;
  sh_task->status = TASK_RUNNING;
}
