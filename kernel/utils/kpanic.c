#include <arch/idt.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <lib/stdtypes.h>

/*
 * API - Stop kernel execution
 */
extern void kpanic(char * message, ...) {
    va_list list;
    va_start(list, message);

    kvprint(message, list);
    asm_lock();
    asm_hlt();
}
