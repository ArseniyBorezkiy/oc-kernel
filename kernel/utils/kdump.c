#include <boot/multiboot.h>
#include <arch/reg.h>
#include <vfs/elf.h>
#include <utils/kdump.h>
#include <utils/kprint.h>
#include <lib/stdtypes.h>

/*
 * Api - Stack dump
 */
extern void kdump_stack(size_t *addr) {
  size_t base = (size_t)addr;

  kprint("-- stack dump: %X\n", base);
  kprint("  %X : %X\n", base, *(u_int*)base);
  kprint("  %X : %X\n", base + 4, *(u_int*)(base + 4));
  kprint("  %X : %X\n", base + 8, *(u_int*)(base + 8));
}

/*
 * Api - Multiboot header dump
 */
extern void kdump_multiboot(struct multiboot_t *header) {
  kprint("-- multiboot header dump: %X\n", (size_t)header);
  kprint("  mem_lower : %X Kb\n", header->mem_lower);
  kprint("  mem_upper : %X Kb\n", header->mem_upper);
}

/*
 * Api - Elf header dump
 */
extern void kdump_elf(struct elf_header_t *header) {
  kprint("-- elf header dump: %X\n", (size_t)header);
  kprint("  entry point: %X\n", header->e_entry);
  kprint("  headers count: %X\n", header->e_phnum);
  kprint("  headers begin: %X\n", header->e_shoff);

  for (int i = 0; i < header->e_phnum; ++i) {
    struct elf_program_header_t *p_header = header->e_shoff + i * header->e_ehsize;
    kprint("  + program header dump: %X\n", p_header);
    kprint("    segment type: %X\n", p_header->p_type);
    kprint("    target virtual address: %X\n", p_header->p_vaddr);
    kprint("    segment size in memory: %X\n", p_header->p_memsz);
    kprint("    segment offset from file begin: %X\n", p_header->p_offset);
    kprint("    segment size in file: %X\n", p_header->p_filesz);
    kprint("    alignment: %X\n", p_header->p_align);
  }
}

/*
 * Api - Dump control registers
 */
extern void kdump_registers() {
  kprint("-- dump control registers\n");
  u16 cs = asm_get_cs();
  u16 ss = asm_get_ss();
  u16 ds = asm_get_ds();
  u32 esp = asm_get_esp();
  u32 eflags = asm_get_eflags();
  u32 cr0 = asm_get_cr0();
  u32 cr3 = asm_get_cr3();
  kprint("  cs = %X  ds = %X  ss = %X\n", cs, ds, ss);
  kprint("  esp = %X\n", esp);
  kprint("  cr0 = %X\n", cr0);
  kprint("  cr3 = %X\n", cr3);
  kprint("  eflags = %X\n", eflags);
}
