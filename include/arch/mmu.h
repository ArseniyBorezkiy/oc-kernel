#pragma once

#include <lib/stdtypes.h>

struct page_directory_entry_t {
    u8 present : 1;
    u8 read_write : 1;
    u8 user_supervisor : 1;
    u8 write_through : 1;
    u8 cache_disabled : 1;
    u8 accessed : 1;
    u8 zero : 1;
    u8 page_size : 1;
    u8 ignored : 1;
    u8 available : 3;
    u32 page_table_addr : 20; /* 4Kb */
} attribute(packed);

struct page_table_entry_t {
    u8 present : 1;
    u8 read_write : 1;
    u8 user_supervisor : 1;
    u8 write_through : 1;
    u8 cache_disabled : 1;
    u8 accessed : 1;
    u8 dirty : 1;
    u8 zero : 1;
    u8 global : 1;
    u8 available : 3;
    u32 page_phys_addr : 20; /* 4Kb | 4MB */
} attribute(packed);

#define MMU_PAGE_TABLE_ENTRIES_COUNT 1024
#define MMU_KERNEL_PAGES_COUNT 768

/*
 * Api
 */
extern void mmu_init();
extern void mmu_enable();
extern void mmu_dump();
extern struct page_directory_entry_t* mmu_get_kdirectory();
extern struct page_table_entry_t* mmu_get_ktable();
extern struct page_table_entry_t* mmu_create_user_page_table();
extern struct page_directory_entry_t* mmu_create_user_page_directory(struct page_table_entry_t* page_table);
extern void mmu_destroy_user_page_directory(struct page_directory_entry_t* upage_dir, struct page_table_entry_t* upage_table);
extern bool mmu_occupy_user_page(struct page_table_entry_t* upage_table, void* phys_addr);
extern void mmu_set_active_page_directory(struct page_directory_entry_t* upage_dir);
