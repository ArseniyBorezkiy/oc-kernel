#include <boot/multiboot.h>
#include <arch/dt.h>
#include <arch/ih.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <utils/kheap.h>
#include <kernel.h>

void *kernel_stack;

/*
 * Api - Kernel entry point
 */
extern void kernel_start(struct multiboot_t* multiboot, void* kstack)
{
    printf("Your first kernel %u.%u\n", 1, 0);
    slist_init();
    printf("[KERNEL]: data structures initialized\n");
    kheap_init();
    printf("[KERNEL]: kernel heap initialized\n");

    kernel_stack = kstack;

    gdt_init();
    idt_init();

    printf("[KERNEL]: kernel started\n");
}
