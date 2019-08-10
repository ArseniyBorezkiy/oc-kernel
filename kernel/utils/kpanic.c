#include <arch/idt.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <lib/stdtypes.h>
#include <messages.h>

/*
 * API - Stop kernel execution
 */
extern void kpanic(char *message, ...)
{
    va_list list;
    va_start(list, message);

    kmode(true);
    kvprintf(message, list);
    kprintf(MSG_KERNEL_PANIC);
    asm_lock();
    asm_hlt();
}
