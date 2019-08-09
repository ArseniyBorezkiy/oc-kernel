#include <sync/spin.h>
#include <utils/kheap.h>
#include <utils/kpanic.h>
#include <utils/kassert.h>
#include <utils/kprint.h>
#include <utils/kdump.h>
#include <lib/string.h>
#include <lib/stdtypes.h>
#include <kernel.h>
#include <messages.h>

struct kheap_entry_t kheap_blocks[KHEAP_MAX_ENTRIES];
struct slist_definition_t kheap_list = {
    .head = null,
    .tail = null,
    .slot_size = sizeof(struct slist_head_t),
    .slots = KHEAP_MAX_ENTRIES,
    .base = (size_t)kheap_blocks};

static void kheap_test();

/*
 * Api - Kernel memory init
 */
extern void kheap_init()
{
    /* init data structures */
    memset(kheap_blocks, 0, sizeof(struct kheap_entry_t) * KHEAP_MAX_ENTRIES);
    /* run tests */
    kheap_test();
}

/*
 * Api - Kernel memory alloc
 */
extern void *kmalloc(size_t size)
{
    struct kheap_entry_t *current = null;
    struct kheap_entry_t *head = (struct kheap_entry_t *)kheap_list.head;

    /* try to use free block */
    for (current = head; current != null; current = (struct kheap_entry_t *)current->list_head->next)
    {
        /* check size is enough */
        if (current->size < size)
        {
            /* try to ask contribution from free left sibling */
            if (current->list_head->prev != null)
            {
                /* left sibling has found */
                struct kheap_entry_t *sibling;
                sibling = (struct kheap_entry_t *)current->list_head->prev;
                /* check whether left sibling is free */
                if (!sibling->is_buzy)
                {
                    /* ask lack from left sibling */
                    size_t lack = size - current->size;
                    sibling->size -= lack;
                    current->addr -= lack;
                    current->size += lack;
                    kassert(__FILE__, __LINE__, current->size == size);
                    /* whether left sibling is collapsed */
                    if (sibling->size == 0)
                    {
                        slist_delete_entry(&kheap_list, (struct slist_head_t *)sibling);
                    }
                    /* occupy block */
                    current->is_buzy = true;
                    kassert(__FILE__, __LINE__, current->addr >= KHEAP_START_ADDR);
                    kassert(__FILE__, __LINE__, current->addr < KHEAP_END_ADDR);
                    return (void *)current->addr; /* suitable block has found */
                }
            }
            else if (current->list_head->next == null)
            {
                /* try to allocate new memory */
                size_t heap_end_addr = current->addr + current->size;
                size_t lack = size - current->size;
                /* check free memory size is enought */
                if (heap_end_addr + lack < KHEAP_END_ADDR)
                {
                    /* occupy block */
                    current->size += lack;
                    current->is_buzy = true;
                    kassert(__FILE__, __LINE__, current->addr >= KHEAP_START_ADDR);
                    kassert(__FILE__, __LINE__, current->addr < KHEAP_END_ADDR);
                    return (void *)current->addr; /* suitable block has found */
                }
            }
        }
        else
        {
            /* occupy block */
            current->is_buzy = true;
            /* try to contribute free right sibling */
            if (current->list_head->next != null)
            {
                /* sibling has found */
                struct kheap_entry_t *sibling;
                sibling = (struct kheap_entry_t *)current->list_head->next;
                size_t surplus = current->size - size;
                /* check whether sibling is free */
                if (!sibling->is_buzy)
                {
                    /* give surplus to right sibling */
                    current->size -= surplus;
                    sibling->addr -= surplus;
                    sibling->size += surplus;
                    kassert(__FILE__, __LINE__, current->size == size);
                }
                else
                {
                    /* give surplus to new right sibling */
                    struct kheap_entry_t *new_sibling;
                    new_sibling = (struct kheap_entry_t *)slist_insert_entry_after(&kheap_list, (struct slist_head_t *)current);
                    if (new_sibling != null)
                    {
                        current->size -= surplus;
                        new_sibling->is_buzy = false;
                        new_sibling->addr = current->addr + current->size;
                        new_sibling->size = surplus;
                    }
                }
            }
            kassert(__FILE__, __LINE__, current->addr >= KHEAP_START_ADDR);
            kassert(__FILE__, __LINE__, current->addr < KHEAP_END_ADDR);
            return (void *)current->addr; /* suitable block has found */
        }
    }

    /* try to alloc new block */
    size_t heap_end_addr = KHEAP_START_ADDR;
    /* calculate heap end address */
    if (kheap_list.tail)
    {
        struct kheap_entry_t *tail;
        tail = (struct kheap_entry_t *)kheap_list.tail;
        heap_end_addr = tail->addr + tail->size;
    }
    /* check free memory size is enought */
    if (heap_end_addr + size >= KHEAP_END_ADDR)
    {
        kpanic(MSG_KERNEL_HEAP_EXCEED);
    }
    /* allocate new heap memory block */
    current = (struct kheap_entry_t *)slist_insert_entry_after(&kheap_list, kheap_list.tail);
    current->addr = heap_end_addr;
    current->size = size;
    current->is_buzy = true;
    kassert(__FILE__, __LINE__, current->addr >= KHEAP_START_ADDR);
    kassert(__FILE__, __LINE__, current->addr < KHEAP_END_ADDR);
    return (void *)current->addr;
}

/*
 * Api - Kernel memory free
 */
extern void kfree(void *addr)
{
    struct kheap_entry_t *current = null;
    struct kheap_entry_t *head = (struct kheap_entry_t *)kheap_list.head;

    for (current = head; current != null; current = (struct kheap_entry_t *)current->list_head->next)
    {
        if (current->addr == (size_t)addr && current->is_buzy)
        {
            struct kheap_entry_t *prev;
            struct kheap_entry_t *next;
            prev = (struct kheap_entry_t *)current->list_head->prev;
            next = (struct kheap_entry_t *)current->list_head->next;

            /* free block */
            current->is_buzy = false;
            /* try to merge with free left sibling */
            if (prev != null && !prev->is_buzy)
            {
                prev->size += current->size;
                slist_delete_entry(&kheap_list, (struct slist_head_t *)current);
            }
            /* try to merge with free right sibling */
            if (next != null && !next->is_buzy)
            {
                current->size += next->size;
                slist_delete_entry(&kheap_list, (struct slist_head_t *)next);
            }

            return;
        }
    }

    kpanic("invalid kernel heap address to free %X", addr);
}

/*
 * Api - Kernel heap dump
 */
extern void kheap_dump() {
  kprint("-- dump kernel heap\n");
  
  struct kheap_entry_t *current;
  struct kheap_entry_t *head = (struct kheap_entry_t *)kheap_list.head;

  for (current = head; current != null; current = (struct kheap_entry_t *)current->list_head->next) {
    kprint("  %X - %X bizy=%u prev=%X next=%X\n", current->addr, current->addr + current->size, current->is_buzy, current->list_head->prev, current->list_head->next);
  }
}

/*
 * Smoke test
 */
static void kheap_test()
{
#ifdef TEST
    /* allocate 3 small blocks */
    void *addr1 = kmalloc(16);
    kassert(__FILE__, __LINE__, addr1 == (void *)KHEAP_START_ADDR);
    void *addr2 = kmalloc(16);
    kassert(__FILE__, __LINE__, addr2 == (void *)addr1 + 16);
    void *addr3 = kmalloc(16);
    kassert(__FILE__, __LINE__, addr3 == (void *)addr2 + 16);
    /* free middle block */
    kfree(addr2);
    /* allocate 2 tiny block */
    void *addr4 = kmalloc(8);
    kassert(__FILE__, __LINE__, addr4 == (void *)addr1 + 16);
    void *addr5 = kmalloc(6);
    kassert(__FILE__, __LINE__, addr5 == (void *)(addr4 + 8));
    /* allocate small block */
    void *addr6 = kmalloc(16);
    kassert(__FILE__, __LINE__, addr6 == (void *)(addr3 + 16));
    /* free first and last block */
    kfree(addr1);
    kfree(addr6);
    /* allocte 2 tiny block */
    void *addr7 = kmalloc(4);
    kassert(__FILE__, __LINE__, addr7 == (void *)(addr1));
    void *addr8 = kmalloc(4);
    kassert(__FILE__, __LINE__, addr8 == (void *)(addr1 + 4));
    /* allocate big block */
    void *addr9 = kmalloc(64);
    kassert(__FILE__, __LINE__, addr9 == (void *)(KHEAP_START_ADDR + 16 + 16));
    /* free all blocks */
    kfree(addr3);
    kfree(addr4);
    kfree(addr5);
    kfree(addr7);
    kfree(addr8);
    kfree(addr9);
    /* allocate 1 tiny block */
    void *addr10 = kmalloc(1);
    kassert(__FILE__, __LINE__, addr9 == (void *)KHEAP_START_ADDR);
    kfree(addr10);
    /* clear heap table */
    memset(kheap_blocks, 0, sizeof(struct kheap_entry_t) * KHEAP_MAX_ENTRIES);
    kprint("heap test ok!");
#endif
}
