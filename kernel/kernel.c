#include <boot/multiboot.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <utils/kheap.h>

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
}
