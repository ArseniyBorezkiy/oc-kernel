#include <tasks/tty.h>
#include <utils/kprint.h>
#include <lib/time.h>
#include <arch/reg.h>
#include <arch/idt.h>
#include <messages.h>

/*
 * Api - Teletype task
 */
extern void task_tty() {
    while (1) {
        kprint(MSG_TASK_TTY_LAUNCHED, (size_t*)asm_get_sp());
        delay(1);
    }
}
