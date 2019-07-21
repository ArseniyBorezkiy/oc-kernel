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
void idt_init();
void asm_idt_load(size_t *addr);
void idt_fill_entry(u_char offset, size_t addr);
void ih_keyboard();
void asm_ih_keyboard();
void asm_ih_timer(size_t *ret_addr);
void asm_lock();
void asm_unlock();
void asm_hlt();
