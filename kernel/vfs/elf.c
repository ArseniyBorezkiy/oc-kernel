#include <vfs/elf.h>
#include <lib/assert.h>
#include <lib/stdio.h>
#include <messages.h>

/*
 * Api - load elf file to memory
 */
extern void elf_load(struct elf_header_t *elf) {
  assert(elf->e_ident.ei_magic == EI_MAGIC);

  printf(MSG_KERNEL_ELF_LOADING, elf->e_phnum);

  for (int i = 0; i < elf->e_phnum; ++i) {

  }
}
