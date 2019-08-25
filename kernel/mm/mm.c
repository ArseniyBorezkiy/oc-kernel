#include <mm/mm.h>
#include <lib/assert.h>

/*
 * Api - allocate elf file in free memory
 */
extern void mm_alloc_elf(struct elf_header_t *elf) {
  assert(elf->e_ident.ei_magic == EI_MAGIC);
}
