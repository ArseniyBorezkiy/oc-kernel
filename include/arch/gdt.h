#pragma once

#include <lib/stdtypes.h>

#define GDT_NULL_SEGMENT 0
#define GDT_KCODE_SEGMENT 1
#define GDT_KDATA_SEGMENT 2
#define GDT_KSTACK_SEGMENT 3
#define GDT_UCODE_SEGMENT 4
#define GDT_UDATA_SEGMENT 5
#define GDT_USTACK_SEGMENT 6
#define GDT_TSS_SEGMENT 7
#define GDT_ENTRIES_COUNT 8

#define GDT_NULL_SELECTOR 0x00
#define GDT_KCODE_SELECTOR 0x08
#define GDT_KDATA_SELECTOR 0x10
#define GDT_KSTACK_SELECTOR 0x18
#define GDT_KTSS_SELECTOR 0x38
#define GDT_UCODE_SELECTOR 0x23
#define GDT_UDATA_SELECTOR 0x2b
#define GDT_USTACK_SELECTOR 0x33
#define GDT_UTSS_SELECTOR 0x3b

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
 * Global descriptor table entry
 */
struct GDT_entry_t {
    u16 limit_low: 16;
    u16 base_low: 16;
    u8 base_middle: 8;
    u8 type: 4; /* whether code (0b1010), data (0b0010), stack (0b0110) or tss (0b1001) */
    u8 s: 1; /* whether system descriptor */
    u8 dpl: 2; /* privilege level */
    u8 p: 1; /* whether segment prensent */
    u8 limit_high: 4;
    u8 a: 1; /* reserved for operation system */
    u8 zero: 1; /* zero */
    u8 db: 1; /* whether 16 or 32 segment */
    u8 g: 1; /* granularity */
    u8 base_high: 8;
} attribute(packed);

/*
 * Global descriptor table pointer
 */
struct GDT_pointer_t {
    u16 limit;
    u32 base;
} attribute(packed);

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
extern void gdt_init();
extern void idt_init();
extern void tss_set_kernel_stack(void *esp0);
extern void asm_switch_context(u32 esp, u32 cr3);
extern void asm_tss_load(u_int index);
