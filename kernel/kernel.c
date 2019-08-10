#include <boot/multiboot.h>
#include <arch/idt.h>
#include <arch/pic.h>
#include <arch/mmu.h>
#include <data/slist.h>
#include <dev/video.h>
#include <sched/task.h>
#include <sched/sched.h>
#include <tasks/tty.h>
#include <tasks/init.h>
#include <tasks/sh.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <utils/kassert.h>
#include <utils/kdump.h>
#include <utils/kheap.h>
#include <utils/lib.h>
#include <lib/time.h>
#include <lib/stdtypes.h>
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
  kclear();

  /* hello */
  kprint(MSG_KERNEL_START, &kernel_start, multiboot->mem_upper);

  /* init arch */
  lib_init();
  idt_init();
  mmu_init();

  /* init data structures */
  slist_init();

  /* init dynamic memory */
  kheap_init();
  
  /* init scheduler */
  sched_init();
  kprint(MSG_KERNEL_SCHEDULER_INITIALIZED);
  kernel_create_tasks();
  kernel_run_tasks();

  /* enable interrupts */
  pic_enable();
  asm_unlock();

  /* start scheduler */
  kprint(MSG_KERNEL_STARTED);
  sched_yield();
  kunreachable(__FILE__, __LINE__);
}

/*
 * Create kernel tasks
 */
static void kernel_create_tasks()
{
  bool is_ok =
      task_create(TID_INIT, task_init_main) &&
      task_create(TID_TTY, task_tty_main) &&
      task_create(TID_TTY, task_sh_main);
  kassert(__FILE__, __LINE__, is_ok);
}

/*
 * Run kernel tasks
 */
static void kernel_run_tasks()
{
  struct sched_task_t *init_task;
  struct sched_task_t *tty_task;
  struct sched_task_t *sh_task;

  init_task = task_get_by_id(TID_INIT);
  tty_task = task_get_by_id(TID_TTY);
  sh_task = task_get_by_id(TID_SH);

  init_task->status = TASK_RUNNING;
  tty_task->status = TASK_RUNNING;
  sh_task->status = TASK_RUNNING;
}
