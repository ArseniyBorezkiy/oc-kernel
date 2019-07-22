#include <tasks/init.h>
#include <utils/kprint.h>
#include <lib/time.h>
#include <arch/idt.h>
#include <messages.h>

/*
 * Api - Init task
 */
extern void task_init() {
    while (1) {
        asm_lock();
        kprint(MSG_TASK_INIT_LAUNCHED);
        asm_unlock();
        delay(1);
    }
}
