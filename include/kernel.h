#pragma once

#include <multiboot.h>

extern void* kernel_stack; /* kernel stack begin */

#define KERNEL_CODE_SEGMENT_SELECTOR 0x08
#define KERNEL_CODE_END_ADDR 0x200000
#define KERNEL_STACK_SIZE 8192

/*
 * Api
 */
extern void kernel_start(struct multiboot_t* multiboot, void* kstack);
