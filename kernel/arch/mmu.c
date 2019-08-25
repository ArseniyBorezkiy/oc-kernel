#include <arch/mmu.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <lib/assert.h>
#include <kernel.h>

extern void asm_enable_paging(void *page_directory);

static struct page_directory_entry_t kpage_directory attribute(aligned(4096));
static struct page_table_entry_t kpage_table[PAGE_TABLE_ENTRIES_COUNT] attribute(aligned(4096));

/*
 * Api - init kernel page directory
 * Here assumed each entry addresses 4Kb
 */
extern void mmu_init()
{
    memset(&kpage_directory, 0, sizeof(struct page_directory_entry_t));

    /* set kernel page directory */
    kpage_directory.zero = 1;
    kpage_directory.accessed = 0;
    kpage_directory.available = 0;
    kpage_directory.cache_disabled = 0;
    kpage_directory.ignored = 0;
    kpage_directory.page_size = 0;
    kpage_directory.present = 1;
    kpage_directory.read_write = 1;
    kpage_directory.user_supervisor = 1;
    kpage_directory.write_through = 1;
    kpage_directory.page_table_addr = (size_t)kpage_table >> 12;

    /* set kernel table */
    for (int i = 0; i < PAGE_TABLE_ENTRIES_COUNT; ++i)
    {
        kpage_table[i].zero = 0;
        kpage_table[i].accessed = 0;
        kpage_table[i].available = 0;
        kpage_table[i].cache_disabled = 0;
        kpage_table[i].dirty = 0;
        kpage_table[i].global = 1;
        kpage_table[i].present = 1;
        kpage_table[i].read_write = 1;
        kpage_table[i].user_supervisor = 1;
        kpage_table[i].write_through = 1;
        kpage_table[i].page_phys_addr = (i * 4096) >> 12; /* assume 4Kb pages */
    }
}

/*
 * Api - Enable paging
 */
extern void mmu_enable() {
  asm_enable_paging(&kpage_directory);
}

/*
 * Api - Get kernel page directory
 */
extern struct page_directory_entry_t *mmu_get_kdirectory() {
  return &kpage_directory;
}

/*
 * Api - Dump kernel page directory
 */
extern void mmu_kdump(u_int table_index) {
  printf(" -- Kernel page directory dump (dir=%X (tables addr %X), tables addr=%X)\n", &kpage_directory, kpage_directory.page_table_addr, &kpage_table);

  assert(kpage_directory.page_table_addr == (size_t)&kpage_table >> 12);

  printf(" table %X\n", &kpage_table[table_index]);
  for (int i = table_index; i < table_index + 8; ++i)
    {
      printf("   phys=%X a=%X g=%X p=%X\n",
        kpage_table[i].page_phys_addr,
        kpage_table[i].available,
        kpage_table[i].global,
        kpage_table[i].present
      );
    }
}
