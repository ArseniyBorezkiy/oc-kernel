#pragma once

extern void *kernel_stack; /* kernel stack begin */

#define KERNEL_HEAP_MEMORY_ADDR 0xC0000000
#define KERNEL_HEAP_MEMORY_SIZE (8 * 1024)
#define KERNEL_STACK_SIZE 8192

/*
 * Api
 */
extern void kernel_start(struct multiboot_t *multiboot, void *kstack);
