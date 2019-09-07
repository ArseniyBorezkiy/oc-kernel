#include <lib/assert.h>
#include <lib/abort.h>
#include <lib/stdio.h>

/*
 * Kernel boolean assertion
 */
extern void kassert(const char* file, u_int line, bool expr)
{
    if (!expr) {
        kpanic("%s line %u\n", file, line);
    }
}

/* 
 * Kernel code unreachable
 */
extern void kunreachable(const char* file, u_int line)
{
    kprintf("code unreachable!\n");
    kpanic("%s line %u\n", file, line);
}
