#include <boot/multiboot.h>
#include <arch/idt.h>
#include <arch/pic.h>
#include <arch/mmu.h>
#include <lib/data/slist.h>
#include <lib/data/clist.h>
#include <dev/utils/video.h>
#include <vfs/initrd.h>
#include <vfs/file.h>
#include <mm/mm.h>
#include <sched/task.h>
#include <sched/sched.h>
#include <dev/tty.h>
#include <tasks/init.h>
#include <tasks/sh.h>
#include <utils/kdump.h>
#include <utils/kheap.h>
#include <utils/lib.h>
#include <lib/time.h>
#include <lib/stdtypes.h>
#include <lib/assert.h>
#include <lib/stdio.h>
#include <lib/string.h>
#include <lib/stdlib.h>
#include <messages.h>
#include <kernel.h>

void *kernel_stack = null;

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
  mm_init();

  /* init devices */
  file_init();
  tty_init();

  /* init scheduler */
  sched_init();

  /* init vfs */
  initrd_autorun(multiboot->mods_addr, multiboot->mods_count);

  /* enable interrupts */
  pic_enable();
  mmu_enable();
  asm_unlock();

  /* start scheduler */
  printf(MSG_KERNEL_STARTED);
  sched_yield();
  unreachable();
}
