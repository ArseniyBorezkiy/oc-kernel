#include <arch/idt.h>
#include <arch/ih.h>
#include <arch/pic.h>
#include <arch/port.h>
#include <kernel.h>
#include <lib/stdio.h>
#include <lib/stdtypes.h>
#include <messages.h>
#include <sched/sched.h>
#include <sched/task.h>

static void idt_fill_entry(u_char offset, size_t addr);
extern void asm_idt_load(size_t* addr);

/*
 * Interrupt descriptor table
 */
static struct IDT_entry IDT[IDT_SIZE];

/*
 * Api - Init interrupt descriptor table
 */
extern void idt_init()
{
    size_t idt_address;
    size_t idt_ptr[2];

    pic_init();

    /* fill idt */
    idt_fill_entry(INT_ZERO, (size_t)asm_ih_zero);
    idt_fill_entry(INT_OPCODE, (size_t)asm_ih_opcode);
    idt_fill_entry(INT_DOUBLE_FAULT, (size_t)asm_ih_double_fault);
    idt_fill_entry(INT_GENERAL_PROTECT, (size_t)asm_ih_general_protect);
    idt_fill_entry(INT_PAGE_FAULT, (size_t)asm_ih_page_fault);
    idt_fill_entry(INT_ALIGNMENT_CHECK, (size_t)asm_ih_alignment_check);
    idt_fill_entry(INT_TIMER, (size_t)asm_ih_timer);
    idt_fill_entry(INT_KEYBOARD, (size_t)asm_ih_keyboard);
    idt_fill_entry(INT_SYSCALL, (size_t)asm_ih_syscall);

    /* load idt */
    idt_address = (size_t)IDT;
    idt_ptr[0] = (LOW_WORD(idt_address) << 16) + (sizeof(struct IDT_entry) * IDT_SIZE);
    idt_ptr[1] = idt_address >> 16;
    asm_idt_load(idt_ptr);
}

/*
 * Fill interrupt descriptor table entry
 */
static void idt_fill_entry(u_char offset, size_t handler)
{
    IDT[offset].offset_lowerbits = LOW_WORD(handler);
    IDT[offset].selector = KERNEL_CODE_SEGMENT_SELECTOR;
    IDT[offset].zero = 0;
    IDT[offset].type_attr = INTERRUPT_GATE;
    IDT[offset].offset_higherbits = HIGH_WORD(handler);
}
