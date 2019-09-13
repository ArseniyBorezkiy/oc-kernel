#pragma once

#ifdef KERNEL
#define malloc(size) kmalloc(size)
#define malloc_a(size, align) kmalloc_a(size, align)
#define free(addr) kfree(addr)
#define abort(addr, ...) kpanic(addr, ##__VA_ARGS__)
#else
/* for user applications */
#endif
