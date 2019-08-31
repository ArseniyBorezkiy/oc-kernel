#include <arch/gdt.h>

extern void asm_gdt_load(void *gdt_ptr);

struct GDT_entry_t gdt[GDT_ENTRIES_COUNT];
struct GDT_pointer_t gdt_ptr;

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

    /* code segment */
    gdt[GDT_CODE_SEGMENT].base_low = 0;
    gdt[GDT_CODE_SEGMENT].base_middle = 0;
    gdt[GDT_CODE_SEGMENT].base_high = 0;
    gdt[GDT_CODE_SEGMENT].limit_low = 0xFFFF;
    gdt[GDT_CODE_SEGMENT].limit_high = 0x0F;
    gdt[GDT_CODE_SEGMENT].type = 0b1010;
    gdt[GDT_CODE_SEGMENT].zero = 0;
    gdt[GDT_CODE_SEGMENT].p = 1;
    gdt[GDT_CODE_SEGMENT].a = 0;
    gdt[GDT_CODE_SEGMENT].g = 1;
    gdt[GDT_CODE_SEGMENT].s = 1;
    gdt[GDT_CODE_SEGMENT].dpl = 0;
    gdt[GDT_CODE_SEGMENT].db = 1;

    /* data segment */
    gdt[GDT_DATA_SEGMENT].base_low = 0;
    gdt[GDT_DATA_SEGMENT].base_middle = 0;
    gdt[GDT_DATA_SEGMENT].base_high = 0;
    gdt[GDT_DATA_SEGMENT].limit_low = 0xFFFF;
    gdt[GDT_DATA_SEGMENT].limit_high = 0x0F;
    gdt[GDT_DATA_SEGMENT].type = 0b0010;
    gdt[GDT_DATA_SEGMENT].zero = 0;
    gdt[GDT_DATA_SEGMENT].p = 1;
    gdt[GDT_DATA_SEGMENT].a = 0;
    gdt[GDT_DATA_SEGMENT].g = 1;
    gdt[GDT_DATA_SEGMENT].s = 1;
    gdt[GDT_DATA_SEGMENT].dpl = 0;
    gdt[GDT_DATA_SEGMENT].db = 1;

    /* stack segment */
    gdt[GDT_STACK_SEGMENT].base_low = 0;
    gdt[GDT_STACK_SEGMENT].base_middle = 0;
    gdt[GDT_STACK_SEGMENT].base_high = 0;
    gdt[GDT_STACK_SEGMENT].limit_low = 0xFFFF;
    gdt[GDT_STACK_SEGMENT].limit_high = 0x0F;
    gdt[GDT_STACK_SEGMENT].type = 0b0110;
    gdt[GDT_STACK_SEGMENT].zero = 0;
    gdt[GDT_STACK_SEGMENT].p = 1;
    gdt[GDT_STACK_SEGMENT].a = 0;
    gdt[GDT_STACK_SEGMENT].g = 1;
    gdt[GDT_STACK_SEGMENT].s = 1;
    gdt[GDT_STACK_SEGMENT].dpl = 0;
    gdt[GDT_STACK_SEGMENT].db = 1;

    /* tss segment */
    gdt[GDT_TSS_SEGMENT].base_low = 0;
    gdt[GDT_TSS_SEGMENT].base_middle = 0;
    gdt[GDT_TSS_SEGMENT].base_high = 0;
    gdt[GDT_TSS_SEGMENT].limit_low = 0xFFFF;
    gdt[GDT_TSS_SEGMENT].limit_high = 0x0F;
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
