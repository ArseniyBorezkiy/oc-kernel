#include <vfs/initrd.h>
#include <vfs/elf.h>
#include <mm/mm.h>
#include <lib/assert.h>
#include <lib/stdio.h>
#include <messages.h>

/*
 * Api - run initial ram disk elf file
 */
extern void initrd_autorun(struct mod_addr_t *mods_addr, int mods_count) {
  assert(mods_count > 0);

  u32 start = mods_addr[0].start;
  u32 end = mods_addr[0].end;

  printf(MSG_KERNEL_INITRD_AREA, start, end);

  /* here assumed inird as usual .elf file */
  elf_load((struct elf_header_t *)start);
}
