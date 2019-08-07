#include <arch/mmu.h>
#include <lib/string.h>
#include <kernel.h>

struct page_directory_entry_t page_directory[PAGE_DIRECTORY_ENTRIES_COUNT];
struct page_table_entry_t page_table[PAGE_DIRECTORY_ENTRIES_COUNT][PAGE_TABLE_ENTRIES_COUNT];

/*
 * Api - init kernel page directory
 * Here assumed each entry addresses 4MB
 */
extern void mmu_init() {
    u_int kpdi = KERNEL_PAGE_DIRECTORY_INDEX;

    /* set kernel page directory */
    page_directory[kpdi].zero = 1;
    page_directory[kpdi].accessed = 0;
    page_directory[kpdi].available = 0;
    page_directory[kpdi].cache_disabled = 0;
    page_directory[kpdi].ignored = 0;
    page_directory[kpdi].page_size = 0;
    page_directory[kpdi].present = 1;
    page_directory[kpdi].read_write = 1;
    page_directory[kpdi].user_supervisor = 1;
    page_directory[kpdi].write_through = 1;
    page_directory[kpdi].page_table_addr = (size_t)&page_table[kpdi];

    /* set kernel table */
    for (int i = 0; i < PAGE_TABLE_ENTRIES_COUNT; ++i) {
        page_table[kpdi][i].zero = 0;
        page_table[kpdi][i].accessed = 0;
        page_table[kpdi][i].available = 0;
        page_table[kpdi][i].cache_disabled = 0;
        page_table[kpdi][i].dirty = 0;
        page_table[kpdi][i].global = 1;
        page_table[kpdi][i].present = 1;
        page_table[kpdi][i].read_write = 1;
        page_table[kpdi][i].user_supervisor = 1;
        page_table[kpdi][i].write_through = 1;
        page_table[kpdi][i].page_phys_addr = i * 1024; /* assume 4Kb pages */
    }
}

/*
 * Api - Set page directory entry
 */
extern void set_page_directory_entry(struct page_directory_entry_t *entry, u_int index) {
    memcpy(&page_directory[index], entry, sizeof(struct page_directory_entry_t));
}

/*
 * Api - Set page table entry
 */
extern void set_page_table_entry(struct page_table_entry_t *entry, u_int dir_index, u_int tbl_index) {
    memcpy(&page_table[dir_index][tbl_index], entry, sizeof(struct page_table_entry_t));
}
