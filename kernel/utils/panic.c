#include <utils/kprint.h>
#include <utils/panic.h>
#include <arch/idt.h>

/*
 * Stop kernel execution
 */
void panic(char * message) {
    kprint(message);
    asm_lock();
    asm_hlt();
}

