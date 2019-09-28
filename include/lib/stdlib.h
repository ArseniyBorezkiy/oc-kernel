#pragma once

#include <lib/assembly.h>

#ifdef KERNEL
#include <utils/kheap.h>
#include <utils/kpanic.h>
#define malloc(size) kmalloc(size)
#define malloc_a(size, align) kmalloc_a(size, align)
#define free(addr) kfree(addr)
#define free_a(addr) kfree_a(addr)
#define abort(addr, ...) kpanic(addr, ##__VA_ARGS__)
#else
#define malloc(size) umalloc(size)
#define free(size) ufree(size)
#define abort(addr, ...) upanic(addr, ##__VA_ARGS__)
#endif
