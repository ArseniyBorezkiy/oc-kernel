#include <arch/gdt.h>
#include <arch/tss.h>
#include <lib/string.h>

extern void asm_gdt_load(void *gdt_ptr);

struct GDT_entry_t gdt[GDT_ENTRIES_COUNT];
struct GDT_pointer_t gdt_ptr;
struct TSS_entry_t tss;

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

    /* load */
    asm_gdt_load(&gdt_ptr);
}
