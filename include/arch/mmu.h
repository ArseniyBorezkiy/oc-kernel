#pragma once

#include <lib/stdtypes.h>

struct page_directory_entry_t {
    u8 present: 1;
    u8 read_write: 1;
    u8 user_supervisor: 1;
    u8 write_through: 1;
    u8 cache_disabled: 1;
    u8 accessed: 1;
    u8 zero: 1;
    u8 page_size: 1;
    u8 ignored: 1;
    u8 available: 3;
    u32 page_table_addr: 20; /* 4Kb */
} attribute(packed);

struct page_table_entry_t {
    u8 present: 1;
    u8 read_write: 1;
    u8 user_supervisor: 1;
    u8 write_through: 1;
    u8 cache_disabled: 1;
    u8 accessed: 1;
    u8 dirty: 1;
    u8 zero: 1;
    u8 global: 1;
    u8 available: 3;
    u32 page_phys_addr: 20; /* 4Kb | 4MB */
} attribute(packed);

#define PAGE_DIRECTORY_ENTRIES_COUNT 4
#define PAGE_TABLE_ENTRIES_COUNT 1024

/*
 * Api
 */
extern void asm_enable_paging(void *page_directory);
extern void mmu_init();
extern void set_page_directory_entry(struct page_directory_entry_t *entry, u_int index);
extern void set_page_table_entry(struct page_table_entry_t *entry, u_int dir_index, u_int tbl_index);
