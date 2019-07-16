#pragma once

#define IDT_SIZE 256
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
 * Helpers
 */
void load_idt(unsigned long *addr);
void keyboard_handler();
void keyboard_init();
