#include <arch/idt.h>
#include <arch/pic.h>
#include <arch/port.h>
#include <sched/task.h>
#include <arch/memory.h>
#include <utils/kprint.h>
#include <messages.h>
#include <types.h>

/*
 * Interrupt descriptor table
 */
struct IDT_entry IDT[IDT_SIZE];

/*
 * Init interrupt descriptor table
 */
void idt_init()
{
    size_t idt_address;
    size_t idt_ptr[2];

    pic_init();

    /* fill idt */
    idt_fill_entry(INT_TIMER, (size_t)asm_ih_timer);
    idt_fill_entry(INT_KEYBOARD, (size_t)asm_ih_keyboard);

    /* load idt */
    idt_address = (size_t)IDT;
    idt_ptr[0] = (LOW_WORD(idt_address) << 16) + (sizeof(struct IDT_entry) * IDT_SIZE);
    idt_ptr[1] = idt_address >> 16;
    asm_idt_load(idt_ptr);
}

/*
 * Fill interrupt descriptor table entry
 */
void idt_fill_entry(u_char offset, size_t handler) {
    IDT[offset].offset_lowerbits = LOW_WORD(handler);
    IDT[offset].selector = CODE_SEGMENT_SELECTOR;
    IDT[offset].zero = 0;
    IDT[offset].type_attr = INTERRUPT_GATE;
    IDT[offset].offset_higherbits = HIGH_WORD(handler);
}

/*
 * Timer interrupt handler
 */
void ih_timer(size_t *ret_addr) {
    write_port(PIC1_CMD_PORT, 0x20); /* end of interrupt */
    sched_schedule(ret_addr); /* schedule next process */
}

/*
 * Keyboard interrupt handler
 */
void ih_keyboard() {
    write_port(PIC1_CMD_PORT, 0x20); /* end of interrupt */
    kprint(MSG_IRQ1);

    u_char status = read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01) {
        read_port(KEYBOARD_DATA_PORT);
    }
}
