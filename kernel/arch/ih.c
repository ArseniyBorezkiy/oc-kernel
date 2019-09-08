#include <arch/dt.h>
#include <arch/ih.h>
#include <arch/pic.h>
#include <arch/port.h>
#include <lib/abort.h>
#include <lib/assert.h>
#include <lib/stdio.h>

/*
 * Api - Double fault
 */
extern void ih_double_fault()
{
    abort("[IH]: double fault\n");
}

/*
 * Api - General protect
 */
extern void ih_general_protect()
{
    abort("[IH]: general protect\n");
}

/*
 * Api - Alignment check
 */
extern void ih_alignment_check()
{
    abort("[IH]: alignment check\n");
}

/*
 * Api - Keyboard interrupt handler
 */
extern void ih_keyboard()
{
    printf("[IH]: irq %u\n", 1);

    u_char status = asm_read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01) {
        char keycode = asm_read_port(KEYBOARD_DATA_PORT);

        if (keycode < 1) {
            goto end;
        }

        /* call low half (bottom) interrupt handler */
    }

end:
    asm_write_port(PIC1_CMD_PORT, 0x20); /* end of interrupt */
}
