#pragma once

#include <lib/assembly.h>

#ifdef KERNEL
#include <utils/kheap.h>
#define malloc(size) kmalloc(size)
#define free(addr) kfree(addr)
#else
#define malloc(size) umalloc(size)
#define free(size) ufree(size)
#endif
