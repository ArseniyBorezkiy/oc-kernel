#include <arch/dt.h>
#include <arch/mmu.h>
#include <arch/pic.h>
#include <boot/multiboot.h>
#include <dev/tty.h>
#include <dev/utils/video.h>
#include <kernel.h>
#include <lib/assert.h>
#include <lib/data/clist.h>
#include <lib/data/slist.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/stdtypes.h>
#include <lib/string.h>
#include <lib/time.h>
#include <messages.h>
#include <mm/mm.h>
#include <sched/sched.h>
#include <sched/task.h>
#include <tasks/dq.h>
#include <utils/kdump.h>
#include <utils/kheap.h>
#include <utils/lib.h>
#include <vfs/file.h>
#include <vfs/initrd.h>

static void create_kernel_tasks();

/*
 * Data
 */
void* kernel_stack = null;

/*
 * Api - Kernel entry point
 */
extern void kernel_start(struct multiboot_t* multiboot, void* kstack)
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
    gdt_init();
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
    create_kernel_tasks();

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

/*
 * Prepare kernel tasks
 */
static void create_kernel_tasks() {
    struct task_mem_t kernel_task_mem;
    kernel_task_mem.pages = null;
    kernel_task_mem.pages_count = 0;
    kernel_task_mem.page_dir = mmu_get_kdirectory();
    kernel_task_mem.page_table = mmu_get_ktable();

    assert(task_create(TID_DQ, dq_task, &kernel_task_mem));
    
    struct task_t *task;
    task = task_get_by_id(TID_DQ);
    strcpy(task->name, dq_task_name);
    task->status = TASK_RUNNING;
}
