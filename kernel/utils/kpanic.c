#include <arch/reg.h>
#include <lib/abort.h>
#include <lib/stdio.h>

/* 
 * API - Stop kernel execution
 */
extern void kpanic(char* message, ...)
{
    va_list list;
    va_start(list, message);

    kmode(true);
    kvprintf(message, list);
    kprintf("kernel panic!\n");
    asm_lock();
    asm_hlt();
}
