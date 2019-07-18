#pragma once

#define IDT_SIZE 256
#define INT_TIMER 0x20
#define INT_KEYBOARD 0x21
#define INTERRUPT_GATE 0x8e

/*
 * Interrupt table entry
 */
struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

/*
 * Api
 */
void idt_init(void);
void asm_idt_load(unsigned long *addr);
void idt_fill_entry(unsigned char offset, unsigned long addr);
void ih_keyboard();
void asm_ih_keyboard();
void asm_ih_timer();
