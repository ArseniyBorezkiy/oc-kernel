#include <arch/reg.h>
#include <arch/idt.h>
#include <tasks/init.h>
#include <utils/kprint.h>
#include <lib/time.h>
#include <lib/stdtypes.h>
#include <messages.h>

/*
 * Api - Init task
 */
extern void task_init() {
    while (1) {
        kprint(MSG_TASK_INIT_LAUNCHED, (size_t*)asm_get_sp());
        delay(1);
    }
}
