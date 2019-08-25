#include <arch/ih.h>
#include <arch/idt.h>
#include <arch/pic.h>
#include <arch/port.h>
#include <sched/sched.h>
#include <sched/task.h>
#include <ipc/ipc.h>
#include <tasks/tty.h>
#include <lib/assert.h>
#include <lib/stdlib.h>
#include <lib/stdtypes.h>
#include <lib/stdio.h>
#include <messages.h>

/*
 * Api - Division by zero
 */
extern void ih_zero()
{
    abort(MSG_INT_DZ);
}

/*
 * Api - Invalid opcode
 */
extern void ih_opcode()
{
    abort(MSG_INT_IO);
}

/*
 * Api - Double fault
 */
extern void ih_double_fault()
{
    abort(MSG_INT_DF);
}

/*
 * Api - General protect
 */
extern void ih_general_protect()
{
    abort(MSG_INT_GP);
}

/*
 * Api - Page fault
 */
extern void ih_page_fault()
{
    abort(MSG_INT_PF);
}

/*
 * Api - Alignment check
 */
extern void ih_alignment_check()
{
    abort(MSG_INT_AC);
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
    printf(MSG_IRQ, 1);

    u_char status = asm_read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01)
    {
        char keycode = asm_read_port(KEYBOARD_DATA_PORT);

        if (keycode < 1) {
          return;
        }

        /* send message to tty task */
        struct message_t msg = {
            .type = TTY_MSG_TYPE_GETC,
            .len = 1,
            .data = {keycode}};
        ksend(TID_TTY, &msg);
    }

    asm_write_port(PIC1_CMD_PORT, 0x20); /* end of interrupt */
}

/*
 * Api - Syscall handler
 */
extern void ih_syscall(u_int *function)
{
    size_t params_addr = ((size_t)function + sizeof(u_int));

    printf(MSG_SYSCALL, *function);

    asm_lock();

    switch (*function) {
      case SYSCALL_KSEND:
        ksend(*(u_int *)params_addr, *(struct message_t **)(params_addr + 4));
        break;
      case SYSCALL_KRECEIVE:
        kreceive(*(u_int *)params_addr, *(struct message_t **)(params_addr + 4));
        break;
      default:
        unreachable();
    }

    asm_unlock();
}
