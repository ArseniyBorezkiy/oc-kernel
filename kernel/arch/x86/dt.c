#include <arch/arch.h>
#include <lib/string.h>
#include <kernel.h>

extern void asm_gdt_load(void *gdt_ptr);
extern void asm_idt_load(size_t* addr);
static void idt_fill_entry(u_char offset, size_t addr);

static struct GDT_entry_t gdt[GDT_ENTRIES_COUNT];
static struct GDT_pointer_t gdt_ptr;
static struct IDT_entry_t IDT[IDT_SIZE];
static struct TSS_entry_t tss;

/*
 * Api - module init
 */
extern void gdt_init() {
    gdt_ptr.base = (size_t)&gdt;
    gdt_ptr.limit = GDT_ENTRIES_COUNT * sizeof(struct GDT_entry_t) - 1;
    
    /* null segment */
    gdt[GDT_NULL_SEGMENT].base_low = 0;
    gdt[GDT_NULL_SEGMENT].base_middle = 0;
    gdt[GDT_NULL_SEGMENT].base_high = 0;
    gdt[GDT_NULL_SEGMENT].limit_low = 0;
    gdt[GDT_NULL_SEGMENT].limit_high = 0;
    gdt[GDT_NULL_SEGMENT].type = 0;
    gdt[GDT_NULL_SEGMENT].zero = 0;
    gdt[GDT_NULL_SEGMENT].p = 0;
    gdt[GDT_NULL_SEGMENT].a = 0;
    gdt[GDT_NULL_SEGMENT].g = 0;
    gdt[GDT_NULL_SEGMENT].s = 0;
    gdt[GDT_NULL_SEGMENT].dpl = 0;
    gdt[GDT_NULL_SEGMENT].db = 0;

    /* kernel code segment */
    gdt[GDT_KCODE_SEGMENT].base_low = 0;
    gdt[GDT_KCODE_SEGMENT].base_middle = 0;
    gdt[GDT_KCODE_SEGMENT].base_high = 0;
    gdt[GDT_KCODE_SEGMENT].limit_low = 0xFFFF;
    gdt[GDT_KCODE_SEGMENT].limit_high = 0x0F;
    gdt[GDT_KCODE_SEGMENT].type = 0b1010;
    gdt[GDT_KCODE_SEGMENT].zero = 0;
    gdt[GDT_KCODE_SEGMENT].p = 1;
    gdt[GDT_KCODE_SEGMENT].a = 0;
    gdt[GDT_KCODE_SEGMENT].g = 1;
    gdt[GDT_KCODE_SEGMENT].s = 1;
    gdt[GDT_KCODE_SEGMENT].dpl = 0;
    gdt[GDT_KCODE_SEGMENT].db = 1;

    /* kernel data segment */
    gdt[GDT_KDATA_SEGMENT].base_low = 0;
    gdt[GDT_KDATA_SEGMENT].base_middle = 0;
    gdt[GDT_KDATA_SEGMENT].base_high = 0;
    gdt[GDT_KDATA_SEGMENT].limit_low = 0xFFFF;
    gdt[GDT_KDATA_SEGMENT].limit_high = 0x0F;
    gdt[GDT_KDATA_SEGMENT].type = 0b0010;
    gdt[GDT_KDATA_SEGMENT].zero = 0;
    gdt[GDT_KDATA_SEGMENT].p = 1;
    gdt[GDT_KDATA_SEGMENT].a = 0;
    gdt[GDT_KDATA_SEGMENT].g = 1;
    gdt[GDT_KDATA_SEGMENT].s = 1;
    gdt[GDT_KDATA_SEGMENT].dpl = 0;
    gdt[GDT_KDATA_SEGMENT].db = 1;

    /* kernel stack segment */
    gdt[GDT_KSTACK_SEGMENT].base_low = 0;
    gdt[GDT_KSTACK_SEGMENT].base_middle = 0;
    gdt[GDT_KSTACK_SEGMENT].base_high = 0;
    gdt[GDT_KSTACK_SEGMENT].limit_low = 0xFFFF;
    gdt[GDT_KSTACK_SEGMENT].limit_high = 0x0F;
    gdt[GDT_KSTACK_SEGMENT].type = 0b0110;
    gdt[GDT_KSTACK_SEGMENT].zero = 0;
    gdt[GDT_KSTACK_SEGMENT].p = 1;
    gdt[GDT_KSTACK_SEGMENT].a = 0;
    gdt[GDT_KSTACK_SEGMENT].g = 1;
    gdt[GDT_KSTACK_SEGMENT].s = 1;
    gdt[GDT_KSTACK_SEGMENT].dpl = 0;
    gdt[GDT_KSTACK_SEGMENT].db = 1;

    /* user code segment */
    gdt[GDT_UCODE_SEGMENT].base_low = 0;
    gdt[GDT_UCODE_SEGMENT].base_middle = 0;
    gdt[GDT_UCODE_SEGMENT].base_high = 0;
    gdt[GDT_UCODE_SEGMENT].limit_low = 0xFFFF;
    gdt[GDT_UCODE_SEGMENT].limit_high = 0x0F;
    gdt[GDT_UCODE_SEGMENT].type = 0b1010;
    gdt[GDT_UCODE_SEGMENT].zero = 0;
    gdt[GDT_UCODE_SEGMENT].p = 1;
    gdt[GDT_UCODE_SEGMENT].a = 0;
    gdt[GDT_UCODE_SEGMENT].g = 1;
    gdt[GDT_UCODE_SEGMENT].s = 1;
    gdt[GDT_UCODE_SEGMENT].dpl = 0b11;
    gdt[GDT_UCODE_SEGMENT].db = 1;

    /* user data segment */
    gdt[GDT_UDATA_SEGMENT].base_low = 0;
    gdt[GDT_UDATA_SEGMENT].base_middle = 0;
    gdt[GDT_UDATA_SEGMENT].base_high = 0;
    gdt[GDT_UDATA_SEGMENT].limit_low = 0xFFFF;
    gdt[GDT_UDATA_SEGMENT].limit_high = 0x0F;
    gdt[GDT_UDATA_SEGMENT].type = 0b0010;
    gdt[GDT_UDATA_SEGMENT].zero = 0;
    gdt[GDT_UDATA_SEGMENT].p = 1;
    gdt[GDT_UDATA_SEGMENT].a = 0;
    gdt[GDT_UDATA_SEGMENT].g = 1;
    gdt[GDT_UDATA_SEGMENT].s = 1;
    gdt[GDT_UDATA_SEGMENT].dpl = 0b11;
    gdt[GDT_UDATA_SEGMENT].db = 1;

    /* user stack segment */
    gdt[GDT_USTACK_SEGMENT].base_low = 0;
    gdt[GDT_USTACK_SEGMENT].base_middle = 0;
    gdt[GDT_USTACK_SEGMENT].base_high = 0;
    gdt[GDT_USTACK_SEGMENT].limit_low = 0xFFFF;
    gdt[GDT_USTACK_SEGMENT].limit_high = 0x0F;
    gdt[GDT_USTACK_SEGMENT].type = 0b0110;
    gdt[GDT_USTACK_SEGMENT].zero = 0;
    gdt[GDT_USTACK_SEGMENT].p = 1;
    gdt[GDT_USTACK_SEGMENT].a = 0;
    gdt[GDT_USTACK_SEGMENT].g = 1;
    gdt[GDT_USTACK_SEGMENT].s = 1;
    gdt[GDT_USTACK_SEGMENT].dpl = 0b11;
    gdt[GDT_USTACK_SEGMENT].db = 1;

    u32 base = (u32)&tss;
    u32 limit = base + sizeof(struct TSS_entry_t);
    memset(&tss, 0, sizeof(struct TSS_entry_t));

    /* tss segment */
    gdt[GDT_TSS_SEGMENT].base_low = LOW_WORD(base);
    gdt[GDT_TSS_SEGMENT].base_middle = LOW_BYTE(HIGH_WORD(base));
    gdt[GDT_TSS_SEGMENT].base_high = HIGH_BYTE(HIGH_WORD(base));
    gdt[GDT_TSS_SEGMENT].limit_low = LOW_WORD(limit);
    gdt[GDT_TSS_SEGMENT].limit_high = LOW_BYTE(HIGH_WORD(limit));
    gdt[GDT_TSS_SEGMENT].type = 0b1001;
    gdt[GDT_TSS_SEGMENT].zero = 0;
    gdt[GDT_TSS_SEGMENT].p = 1;
    gdt[GDT_TSS_SEGMENT].a = 0;
    gdt[GDT_TSS_SEGMENT].g = 1;
    gdt[GDT_TSS_SEGMENT].s = 0;
    gdt[GDT_TSS_SEGMENT].dpl = 0;
    gdt[GDT_TSS_SEGMENT].db = 0;

    /* tss */
    tss.cs = GDT_UCODE_SELECTOR;
    tss.ds = GDT_UDATA_SELECTOR;
    tss.es = GDT_UDATA_SELECTOR;
    tss.fs = GDT_UDATA_SELECTOR;
    tss.gs = GDT_UDATA_SELECTOR;
    tss.ss = GDT_USTACK_SELECTOR;

    tss_set_kernel_stack(kernel_stack);

    /* load */
    asm_gdt_load(&gdt_ptr);
    asm_tss_load(GDT_UTSS_SELECTOR);
}

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
    idt_ptr[0] = (LOW_WORD(idt_address) << 16) + (sizeof(struct IDT_entry_t) * IDT_SIZE);
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

/*
 * Api - Set kernel stack
 */
extern void tss_set_kernel_stack(void *esp0) {
    tss.esp0 = (u32)esp0;
}
