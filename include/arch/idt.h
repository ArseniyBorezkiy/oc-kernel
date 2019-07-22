#pragma once

#include <types.h>

#define IDT_SIZE 256
#define INT_TIMER 0x20
#define INT_KEYBOARD 0x21
#define INTERRUPT_GATE 0x8e

/*
 * Interrupt table entry
 */
struct IDT_entry {
    u16 offset_lowerbits;
    u16 selector;
    u8 zero;
    u8 type_attr;
    u16 offset_higherbits;
};

/*
 * Api
 */
extern void idt_init();
extern void ih_keyboard();
extern void asm_lock();
extern void asm_unlock();
extern void asm_hlt();
