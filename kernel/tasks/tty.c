#include <arch/reg.h>
#include <arch/idt.h>
#include <sched/task.h>
#include <tasks/tty.h>
#include <ipc/ipc.h>
#include <utils/kprint.h>
#include <lib/time.h>
#include <lib/stdtypes.h>
#include <messages.h>

/*
 * Api - Teletype task
 */
extern void task_tty() {
    struct message_t msg;
    
    while (1) {
        kprint(MSG_TASK_TTY_LAUNCHED, (size_t*)asm_get_sp());
        delay(1);
        kreceive(TID_TTY, &msg);
    }
}
