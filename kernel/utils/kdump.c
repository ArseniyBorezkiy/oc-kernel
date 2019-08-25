#include <boot/multiboot.h>
#include <arch/reg.h>
#include <vfs/elf.h>
#include <utils/kdump.h>
#include <utils/kheap.h>
#include <lib/stdtypes.h>
#include <lib/stdio.h>
#include <lib/assert.h>
#include <kernel.h>

/*
 * Api - Stack dump
 */
extern void kdump_stack(size_t *addr)
{
  size_t base = (size_t)addr;

  printf("-- stack dump: %X\n", base);
  printf("  %X : %X\n", base, *(u_int *)base);
  printf("  %X : %X\n", base + 4, *(u_int *)(base + 4));
  printf("  %X : %X\n", base + 8, *(u_int *)(base + 8));
}

/*
 * Api - Multiboot header dump
 */
extern void kdump_multiboot(struct multiboot_t *header)
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
  printf("  kernel stack: %X - %X\n", kernel_stack_addr, kernel_stack_addr - KERNEL_STACK_SIZE);
}
