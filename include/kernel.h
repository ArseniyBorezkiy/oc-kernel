#pragma once

extern void *kernel_stack; /* kernel stack begin */

#define KERNEL_CODE_SEGMENT_SELECTOR 0x08
#define KERNEL_HEAP_MEMORY_ADDR 0xC0000000
#define KERNEL_HEAP_MEMORY_SIZE (8 * 1024)
#define KERNEL_STACK_SIZE 8192
#define KERNEL_PAGE_DIRECTORY_INDEX 0

/*
 * Api
 */
extern void kernel_start(struct multiboot_t *multiboot, void *kstack);
