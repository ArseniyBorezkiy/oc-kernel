#include <arch/arch.h>
#include <multiboot.h>
#include <kernel.h>
#include <lib/assert.h>
#include <lib/stdio.h>
#include <lib/stdtypes.h>
#include <utils/kdump.h>
#include <utils/kheap.h>
#include <vfs/elf.h>

/*
 * Api - Stack dump
 */
extern void kdump_stack(size_t* esp)
{
    size_t base = (size_t)esp;

    printf("-- stack dump: %X\n", base);
    for (int i = 0; i < 8; ++i) {
        printf("  %X : %X\n", base - i * 4, *(u_int*)(base - i * 4));
    }
}

/*
 * Api - Multiboot header dump
 */
extern void kdump_multiboot(struct multiboot_t* header)
{
    printf("-- multiboot header dump: %X\n", (size_t)header);
    printf("  mem_lower : %X Kb\n", header->mem_lower);
    printf("  mem_upper : %X Kb\n", header->mem_upper);
}

/*
 * Api - Dump control registers
 */
extern void kdump_registers()
{
    printf("-- dump control registers\n");
    u16 cs = asm_get_cs();
    u16 ss = asm_get_ss();
    u16 ds = asm_get_ds();
    u32 esp = asm_get_esp();
    u32 eflags = asm_get_eflags();
    u32 cr0 = asm_get_cr0();
    u32 cr3 = asm_get_cr3();
    printf("  cs = %X  ds = %X  ss = %X\n", cs, ds, ss);
    printf("  esp = %X\n", esp);
    printf("  cr0 = %X\n", cr0);
    printf("  cr3 = %X\n", cr3);
    printf("  eflags = %X\n", eflags);
}

/*
 * Api - Dump flags
 */
extern void kdump_eflags()
{
    printf("-- dump flags\n");
    u32 eflags = asm_get_eflags();
    printf("  eflags = %X\n", eflags);
}

/*
 * Api - Dump memory areas
 */
extern void kdump_memory_areas()
{
    printf("-- dump memory areas\n");
    size_t kernel_stack_addr = (size_t)kernel_stack;
    printf("  kernel stack: %X - %X\n",
        kernel_stack_addr,
        kernel_stack_addr - KERNEL_STACK_SIZE);
}
