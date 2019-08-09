#pragma once

#include <lib/stdtypes.h>
#include <data/slist.h>

#define KHEAP_START_ADDR 0x200000
#define KHEAP_END_ADDR 0x300000
#define KHEAP_SIZE (KHEAP_END_ADDR - KHEAP_START_ADDR)
#define KHEAP_MAX_ENTRIES 16

struct kheap_entry_t
{
    struct slist_head_t *list_head; /* should be at first */
    size_t addr;                    /* physical address */
    size_t size;                    /* memory block size */
    bool is_buzy;                   /* whether block used */
};

/*
 * Api
 */
extern void kheap_init();
extern void *kmalloc(size_t size);
extern void kfree(void *addr);
extern struct kheap_entry_t *kheap_table_entries_start();
extern struct kheap_entry_t *kheap_table_entries_next(struct kheap_entry_t *entry);
extern struct kheap_entry_t *kheap_table_entries_end();
