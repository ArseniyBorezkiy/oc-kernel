#include <arch/ih.h>
#include <arch/pic.h>
#include <arch/port.h>
#include <sched/sched.h>
#include <sched/task.h>
#include <ipc/ipc.h>
#include <tasks/tty.h>
#include <utils/kpanic.h>
#include <utils/kprint.h>
#include <lib/stdtypes.h>
#include <messages.h>

/*
 * Api - Division by zero
 */
extern void ih_zero()
{
    kpanic("division by zero!");
}

/*
 * Api - Invalid opcode
 */
extern void ih_opcode()
{
    kpanic("invalid opcode!");
}

/*
 * Api - Double fault
 */
extern void ih_double_fault()
{
    kpanic("double fault!");
}

/*
 * Api - General protect
 */
extern void ih_general_protect()
{
    kpanic("general protect!");
}

/*
 * Api - Page fault
 */
extern void ih_page_fault()
{
    kpanic("page fault!");
}

/*
 * Api - Alignment check
 */
extern void ih_alignment_check()
{
    kpanic("alignment check!");
}

/*
 * Api - Timer interrupt handler
 */
extern void ih_timer(size_t *ret_addr, size_t *reg_addr)
{
    asm_write_port(PIC1_CMD_PORT, 0x20); /* end of interrupt */
    sched_schedule(ret_addr, reg_addr);  /* schedule next process */
}

/*
 * Api - Keyboard interrupt handler
 */
extern void ih_keyboard()
{
    kprint(MSG_IRQ1);

    u_char status = asm_read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01)
    {
        u_char ch = asm_read_port(KEYBOARD_DATA_PORT);

        /* send message to tty task */
        struct message_t msg = {
            .type = TTY_MSG_TYPE_GETC,
            .len = 1,
            .data = {ch}};
        ksend(TID_TTY, &msg);
    }

    asm_write_port(PIC1_CMD_PORT, 0x20); /* end of interrupt */
}
