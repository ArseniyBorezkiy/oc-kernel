#include <arch/ih.h>
#include <arch/pic.h>
#include <arch/port.h>
#include <sched/sched.h>
#include <utils/kprint.h>
#include <messages.h>
#include <types.h>

/*
 * Api - Timer interrupt handler
 */
extern void ih_timer(size_t *ret_addr) {
    write_port(PIC1_CMD_PORT, 0x20); /* end of interrupt */
    sched_schedule(ret_addr); /* schedule next process */
}

/*
 * Api - Keyboard interrupt handler
 */
extern void ih_keyboard() {
    write_port(PIC1_CMD_PORT, 0x20); /* end of interrupt */
    kprint(MSG_IRQ1);

    u_char status = read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01) {
        read_port(KEYBOARD_DATA_PORT);
    }
}
