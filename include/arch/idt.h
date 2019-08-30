#pragma once

#include <lib/stdtypes.h>

#define IDT_SIZE 256
#define INT_ZERO 0x0
#define INT_OPCODE 0x6
#define INT_DOUBLE_FAULT 0x8
#define INT_GENERAL_PROTECT 0xD
#define INT_PAGE_FAULT 0xE
#define INT_ALIGNMENT_CHECK 0x11
#define INT_TIMER 0x20
#define INT_KEYBOARD 0x21
#define INT_SYSCALL 0x80
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
extern void asm_lock();
extern void asm_unlock();
extern void asm_hlt();
