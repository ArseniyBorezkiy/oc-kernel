#include <tasks/tty.h>
#include <utils/kprint.h>
#include <lib/time.h>
#include <arch/idt.h>
#include <messages.h>

/*
 * Api - Teletype task
 */
extern void task_tty() {
    while (1) {
        asm_lock();
        kprint(MSG_TASK_TTY_LAUNCHED);
        asm_unlock();
        delay(1);
    }
}
