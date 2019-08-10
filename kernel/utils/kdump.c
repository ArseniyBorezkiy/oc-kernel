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

  print("-- stack dump: %X\n", base);
  print("  %X : %X\n", base, *(u_int *)base);
  print("  %X : %X\n", base + 4, *(u_int *)(base + 4));
  print("  %X : %X\n", base + 8, *(u_int *)(base + 8));
}

/*
 * Api - Multiboot header dump
 */
extern void kdump_multiboot(struct multiboot_t *header)
{
  print("-- multiboot header dump: %X\n", (size_t)header);
  print("  mem_lower : %X Kb\n", header->mem_lower);
  print("  mem_upper : %X Kb\n", header->mem_upper);
}

/*
 * Api - Elf header dump
 */
extern void kdump_elf(struct elf_header_t *header)
{
  print("-- elf header dump: %X\n", (size_t)header);
  print("  entry point: %X\n", header->e_entry);
  print("  headers count: %X\n", header->e_phnum);
  print("  headers begin: %X\n", header->e_shoff);

  for (int i = 0; i < header->e_phnum; ++i)
  {
    struct elf_program_header_t *p_header = (void *)(header->e_shoff + i * header->e_ehsize);
    print("  + program header dump: %X\n", p_header);
    print("    segment type: %X\n", p_header->p_type);
    print("    target virtual address: %X\n", p_header->p_vaddr);
    print("    segment size in memory: %X\n", p_header->p_memsz);
    print("    segment offset from file begin: %X\n", p_header->p_offset);
    print("    segment size in file: %X\n", p_header->p_filesz);
    print("    alignment: %X\n", p_header->p_align);
  }
}

/*
 * Api - Dump control registers
 */
extern void kdump_registers()
{
  print("-- dump control registers\n");
  u16 cs = asm_get_cs();
  u16 ss = asm_get_ss();
  u16 ds = asm_get_ds();
  u32 esp = asm_get_esp();
  u32 eflags = asm_get_eflags();
  u32 cr0 = asm_get_cr0();
  u32 cr3 = asm_get_cr3();
  print("  cs = %X  ds = %X  ss = %X\n", cs, ds, ss);
  print("  esp = %X\n", esp);
  print("  cr0 = %X\n", cr0);
  print("  cr3 = %X\n", cr3);
  print("  eflags = %X\n", eflags);
}

/*
 * Api - Dump flags
 */
extern void kdump_eflags()
{
  print("-- dump flags\n");
  u32 eflags = asm_get_eflags();
  print("  eflags = %X\n", eflags);
}

/*
 * Api - Dump memory areas
 */
extern void kdump_memory_areas()
{
  print("-- dump memory areas\n");
  size_t kernel_stack_addr = (size_t)kernel_stack;
  print("  kernel stack: %X - %X\n", kernel_stack_addr, kernel_stack_addr - KERNEL_STACK_SIZE);
}
