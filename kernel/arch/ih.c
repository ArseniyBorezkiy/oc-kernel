#include <arch/ih.h>
#include <arch/idt.h>
#include <arch/pic.h>
#include <arch/port.h>
#include <sched/sched.h>
#include <sched/task.h>
#include <ipc/ipc.h>
#include <dev/utils/ih_low.h>
#include <dev/tty.h>
#include <vfs/file.h>
#include <utils/kdump.h>
#include <lib/assert.h>
#include <lib/stdlib.h>
#include <lib/stdtypes.h>
#include <lib/stdio.h>
#include <lib/syscall.h>
#include <messages.h>

void dev_each_low_ih_cb(struct dev_t *entry, struct ih_low_data_t *data);

/*
 * Api - Division by zero
 */
extern void ih_zero()
{
    kdump_stack((void *)asm_get_esp());
    abort(MSG_INT_DZ);
}

/*
 * Api - Invalid opcode
 */
extern void ih_opcode()
{
    kdump_stack((void *)asm_get_esp());
    abort(MSG_INT_IO);
}

/*
 * Api - Double fault
 */
extern void ih_double_fault()
{
    kdump_stack((void *)asm_get_esp());
    abort(MSG_INT_DF);
}

/*
 * Api - General protect
 */
extern void ih_general_protect()
{
    kdump_stack((void *)asm_get_esp());
    abort(MSG_INT_GP);
}

/*
 * Api - Page fault
 */
extern void ih_page_fault()
{
    kdump_stack((void *)asm_get_esp());
    abort(MSG_INT_PF, asm_get_cr2());
}

/*
 * Api - Alignment check
 */
extern void ih_alignment_check()
{
    kdump_stack((void *)asm_get_esp());
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

        if (keycode < 1)
        {
            return;
        }

        /* call low half interrupt handlers */
        struct ih_low_data_t ih_low_data;
        ih_low_data.number = INT_KEYBOARD;
        ih_low_data.data = &keycode;
        dev_for_each(dev_each_low_ih_cb, &ih_low_data);
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

    /* handle syscall */
    switch (*function)
    {
    case SYSCALL_KSEND:
    {
        /* send message */
        u_short tid = *(u_int *)params_addr;
        ksend(tid, *(struct message_t **)(params_addr + 4));
        break;
    }
    case SYSCALL_KRECEIVE:
    {
        /* receive message */
        u_short tid = *(u_int *)params_addr;
        kreceive(tid, *(struct message_t **)(params_addr + 4));
        break;
    }
    case SYSCALL_KILL:
    {
        /* kill task */
        u_short tid = *(u_int *)params_addr;
        struct task_t *task = task_get_by_id(tid);
        task->status = TASK_KILLING;
        task->reschedule = true;
        break;
    }
    case SYSCALL_OPEN:
    {
        /* open file */
        char *dev = *(char **)params_addr;
        int mod_rw = *(int *)(params_addr + 4);
        FILE **file = *(FILE ***)(params_addr + 8);
        *file = file_open(dev, mod_rw);
        break;
    }
    case SYSCALL_READ:
    {
        /* read from file */
        struct io_buf_t *io_buf = *(struct io_buf_t **)params_addr;
        char *buff = *(char **)(params_addr + 4);
        u_int size = *(u_int *)(params_addr + 8);
        file_read(io_buf, buff, size);
        break;
    }
    case SYSCALL_WRITE:
    {
        /* write to file */
        struct io_buf_t *io_buf = *(struct io_buf_t **)params_addr;
        char *data = *(char **)(params_addr + 4);
        u_int size = *(u_int *)(params_addr + 8);
        file_write(io_buf, data, size);
        break;
    }
    case SYSCALL_IOCTL:
    {
        /* device specific command */
        struct io_buf_t *io_buf = *(struct io_buf_t **)params_addr;
        int cmd = *(int *)(params_addr + 4);
        file_ioctl(io_buf, cmd);
        break;
    }
    default:
        unreachable();
    }

    asm_unlock();
}

/*
 * Call low half interrupt handler
 */
void dev_each_low_ih_cb(struct dev_t *entry, struct ih_low_data_t *data)
{
    struct clist_head_t *current;
    struct ih_low_t *ih_low;

    for (current = entry->ih_list.head; current != null; current = current->next)
    {
        ih_low = (struct ih_low_t *)current->data;
        if (ih_low->number == data->number)
        {
            /* call interrupt handler */
            ih_low->handler(data->number, data);
        }

        if (current->next == entry->ih_list.head)
        {
            break;
        }
    }
}
