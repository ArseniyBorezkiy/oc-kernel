#include <arch/interrupt.h>
#include <arch/port.h>
#include <utils/kprint.h>
#include <messages.h>
#include <types.h>

/*
 * Interrupt table
 */
struct IDT_entry IDT[IDT_SIZE];

/*
 * Init interrupt descriptor table
 */
void idt_init(void)
{
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    keyboard_address = (unsigned long)keyboard_handler;
    IDT[INT_KEYBOARD].offset_lowerbits = LOW_WORD(keyboard_address);
    IDT[INT_KEYBOARD].selector = CODE_SEGMENT_SELECTOR;
    IDT[INT_KEYBOARD].zero = 0;
    IDT[INT_KEYBOARD].type_attr = INTERRUPT_GATE;
    IDT[INT_KEYBOARD].offset_higherbits = HIGH_WORD(keyboard_address);

    pic_init();

    idt_address = (unsigned long)IDT ;
    idt_ptr[0] = (LOW_WORD(idt_address) << 16) + (sizeof (struct IDT_entry) * IDT_SIZE);
    idt_ptr[1] = idt_address >> 16;

    load_idt(idt_ptr);
}

/*
 * PIC init
 */
void pic_init() {
    write_port(PIC1_CMD_PORT , PIC_IQW_CMD); /* init PIC1 */
    write_port(PIC2_CMD_PORT , PIC_IQW_CMD); /* init PIC2 */

    write_port(PIC1_DATA_PORT , 0x20); /* IQW2 offset */
    write_port(PIC2_DATA_PORT , 0x28); /* IQW2 offset */

    write_port(PIC1_DATA_PORT , 0x00); /* IQW3 no cascade */
    write_port(PIC2_DATA_PORT , 0x00); /* IQW3 no cascade */

    write_port(PIC1_DATA_PORT , 0x01); /* IQW4 no cascade */
    write_port(PIC2_DATA_PORT , 0x01); /* IQW4 no cascade */

    write_port(PIC1_DATA_PORT , 0xff); /* disable irq */
    write_port(PIC2_DATA_PORT , 0xff); /* disable irq */
}

/*
 * Keyboard handler
 */
void keyboard_handler_main() {
    kprint(MSG_IRQ1);
}

/*
 * Init keyboard
 */
void keyboard_init(void)
{
    write_port(0x21 , 0xFD); /* Enable IRQ1 */
}
