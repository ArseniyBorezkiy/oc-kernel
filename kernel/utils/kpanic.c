#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <arch/idt.h>

/*
 * API - Stop kernel execution
 */
extern void kpanic(char * message) {
    kprint(message);
    asm_lock();
    asm_hlt();
}
