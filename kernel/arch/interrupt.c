#include <arch/interrupt.h>
#include <arch/port.h>
#include <utils/kprint.h>
#include <messages.h>

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
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    write_port(0x20 , 0x11);
    write_port(0xA0 , 0x11);

    write_port(0x21 , 0x20);
    write_port(0xA1 , 0x28);

    write_port(0x21 , 0x00);
    write_port(0xA1 , 0x00);

    write_port(0x21 , 0x01);
    write_port(0xA1 , 0x01);

    write_port(0x21 , 0xff);
    write_port(0xA1 , 0xff);

    idt_address = (unsigned long)IDT ;
    idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16 ;

    load_idt(idt_ptr);
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
