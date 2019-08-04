#pragma once

#include <lib/stdtypes.h>

#define KHEAP_START_ADDR 0x200000
#define KHEAP_END_ADDR 0x300000
#define KHEAP_SIZE (KHEAP_END_ADDR - KHEAP_START_ADDR)
#define KHEAP_MAX_ENTRIES 16

struct kernel_heap_entry_t {
    size_t addr; /* physical address */
    size_t size; /* memory block size */
    bool is_buzy; /* whether block used */
    bool is_valid; /* whether table entry is valid */
    struct kernel_heap_entry_t *next; /* next block */
    struct kernel_heap_entry_t *prev; /* prev block */
};

struct kernel_heap_table_t {
    struct kernel_heap_entry_t block[KHEAP_MAX_ENTRIES];
};

/*
 * Api
 */
extern void kheap_init();
extern void *kmalloc(size_t size);
extern void kfree(void *addr);
extern struct kernel_heap_entry_t *kheap_table_entries_start();
extern struct kernel_heap_entry_t *kheap_table_entries_next(struct kernel_heap_entry_t *entry);
extern struct kernel_heap_entry_t *kheap_table_entries_end();
