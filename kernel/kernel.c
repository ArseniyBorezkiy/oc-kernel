#include <boot/multiboot.h>
#include <lib/string.h>
#include <lib/stdio.h>

char *hello = "Hello world!";

/*
 * Api - Kernel entry point
 */
extern void kernel_start(struct multiboot_t* multiboot, void* kstack)
{
    printf("%s", hello);
}
