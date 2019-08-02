#include <arch/idt.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <lib/stdtypes.h>

/*
 * API - Stop kernel execution
 */
extern void kpanic(char * message) {
    kprint(message);
    asm_lock();
    asm_hlt();
}
