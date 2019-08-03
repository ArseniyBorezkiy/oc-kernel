#include <utils/kheap.h>
#include <utils/kpanic.h>
#include <lib/string.h>

struct kernel_heap_table_t kernel_heap_table;

/*
 * Api - Kernel memory init
 */
extern void kheap_init() {
    memset(&kernel_heap_table, 0, sizeof(kernel_heap_table));
}

/*
 * Api - Kernel memory alloc
 */
extern void *kmalloc(size_t size) {
    struct kernel_heap_entry_t * entry;

    for (entry = kheap_table_entries_start();
         entry != kheap_table_entries_end();
         entry = kheap_table_entries_next(entry)
    ) {
        if (entry->addr == null && !entry->is_buzy) {
            entry->is_buzy = true;
            // TODO: ???
            return entry->addr;
        }
    }

    kpanic("kernel heap exceed");
}

/*
 * Api - Kernel memory free
 */
extern void kfree(void *addr) {
    struct kernel_heap_entry_t * entry;

    for (entry = kheap_table_entries_start();
         entry != kheap_table_entries_end();
         entry = kheap_table_entries_next(entry)
    ) {
        if (entry->addr == addr && entry->is_buzy) {
            entry->is_buzy = false;
            entry->addr = null;
            return;
        }
    }

    kpanic("invalid kernel heap address to free");
}

/*
 * Api - Get first kernel heap table entry
 */
extern struct kernel_heap_entry_t *kheap_table_entries_start() {
    return &kernel_heap_table.block[0];
}

/*
 * Api - Get next kernel heap table entry
 */
extern struct kernel_heap_entry_t *kheap_table_entries_next(struct kernel_heap_entry_t *entry) {
    for (int i = 0; i < KHEAP_MAX_ENTRIES; ++i) {
        if (&kernel_heap_table.block[i] == entry) {
            if (i + 1 < KHEAP_MAX_ENTRIES) {
                return &kernel_heap_table.block[i + 1];
            } else {
                return null;
            }
        }
    }

    return null;
}

/*
 * Api - Get last kernel heap table entry
 */
extern struct kernel_heap_entry_t *kheap_table_entries_end() {
    return &kernel_heap_table.block[KHEAP_MAX_ENTRIES - 1];
}
