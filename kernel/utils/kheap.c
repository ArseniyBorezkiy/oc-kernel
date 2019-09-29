#include <kernel.h>
#include <lib/assert.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/stdtypes.h>
#include <lib/string.h>
#include <messages.h>
#include <sync/spin.h>
#include <utils/kdump.h>
#include <utils/kheap.h>

struct kheap_entry_t kheap_blocks[KHEAP_MAX_ENTRIES];
struct slist_definition_t kheap_list = { .head = null,
    .tail = null,
    .slot_size = sizeof(struct kheap_entry_t),
    .slots = KHEAP_MAX_ENTRIES,
    .base = (size_t)kheap_blocks };

static void kheap_test();
static void kheap_test_aligned();

/*
 * Api - Kernel memory init
 */
extern void kheap_init()
{
    /* init data structures */
    memset(kheap_blocks, 0, sizeof(struct kheap_entry_t) * KHEAP_MAX_ENTRIES);
    kheap_list.head = null;
    kheap_list.tail = null;
    /* run tests */
    kheap_test();
    kheap_test_aligned();
}

/*
 * Api - Kernel memory alloc
 */
extern void* kmalloc(size_t size)
{
    struct kheap_entry_t* current_data = null;
    struct slist_head_t* current = null;
    struct slist_head_t* head = kheap_list.head;

    assert(size > 0);

    /* try to use free block */
    for (current = head; current != null; current = current->next) {
        current_data = (struct kheap_entry_t*)current->data;

        if (current_data->is_buzy) {
            continue;
        }

        /* check size is not enough */
        if (current_data->size < size) {
            /* try to ask contribution from free left sibling */
            if (current->prev != null) {
                /* left sibling has found */
                struct slist_head_t* sibling = current->prev;
                struct kheap_entry_t* sibling_data = (struct kheap_entry_t*)sibling->data;
                /* check whether left sibling is free */
                if (!sibling_data->is_buzy) {
                    /* ask lack from left sibling */
                    size_t lack = size - current_data->size;
                    sibling_data->size -= lack;
                    current_data->addr -= lack;
                    current_data->size += lack;
                    assert(current_data->size == size);
                    /* whether left sibling is collapsed */
                    if (sibling_data->size == 0) {
                        slist_delete_entry(&kheap_list, sibling);
                    }
                    /* occupy block */
                    current_data->is_buzy = true;
                    assert(current_data->addr >= KHEAP_START_ADDR);
                    assert(current_data->addr < KHEAP_END_ADDR);
                    return (void*)current_data->addr; /* suitable block has found */
                }
            }
            /* try to extend borders */
            if (current->next == null) {
                size_t heap_end_addr = current_data->addr + current_data->size;
                size_t lack = size - current_data->size;
                /* check free memory size is enought */
                if (heap_end_addr + lack < KHEAP_END_ADDR) {
                    /* occupy block */
                    current_data->size += lack;
                    current_data->is_buzy = true;
                    assert(current_data->addr >= KHEAP_START_ADDR);
                    assert(current_data->addr < KHEAP_END_ADDR);
                    return (void*)current_data->addr; /* suitable block has found */
                }
            }
        } else {
            /* occupy block */
            current_data->is_buzy = true;
            size_t surplus = current_data->size - size;
            bool is_sibling_bizy = false;
            /* try to contribute free right sibling */
            if (current->next != null) {
                /* sibling has found */
                struct slist_head_t* sibling = current->next;
                struct kheap_entry_t* sibling_data = (struct kheap_entry_t*)sibling->data;

                /* check whether sibling is free */
                is_sibling_bizy = sibling_data->is_buzy;
                if (!is_sibling_bizy) {
                    /* give surplus to right sibling */
                    current_data->size -= surplus;
                    sibling_data->addr -= surplus;
                    sibling_data->size += surplus;
                    assert(current_data->size == size);
                }
            }
            /* try to give surplus to new right sibling */
            if (current->next == null || is_sibling_bizy) {
                {
                    struct slist_head_t* new_sibling;
                    new_sibling = slist_insert_entry_after(&kheap_list, current);
                    struct kheap_entry_t* sibling_data = (struct kheap_entry_t*)new_sibling->data;

                    if (new_sibling != null) {
                        /* give surplus to new right sibling */
                        assert((size_t)new_sibling == (size_t)sibling_data);
                        current_data->size -= surplus;
                        assert(current_data->size > 0);
                        sibling_data->is_buzy = false;
                        sibling_data->addr = current_data->addr + current_data->size;
                        sibling_data->size = surplus;
                    }
                }
            }
            assert(current_data->addr >= KHEAP_START_ADDR);
            assert(current_data->addr < KHEAP_END_ADDR);
            return (void*)current_data->addr; /* suitable block has found */
        }
    }

    /* try to alloc new block */
    size_t heap_end_addr = KHEAP_START_ADDR;
    /* calculate heap end address */
    if (kheap_list.tail) {
        current = kheap_list.tail;
        current_data = (struct kheap_entry_t*)current->data;
        heap_end_addr = current_data->addr + current_data->size;
    }
    /* check free memory size is enought */
    if (heap_end_addr + size >= KHEAP_END_ADDR) {
        abort(MSG_KERNEL_HEAP_EXCEED);
    }
    /* allocate new heap memory block */
    struct slist_head_t* tail = kheap_list.tail;
    current = slist_insert_entry_after(&kheap_list, kheap_list.tail);
    current_data = (struct kheap_entry_t*)current->data;
    assert((size_t)current == (size_t)current_data);
    current_data->addr = heap_end_addr;
    current_data->size = size;
    current_data->is_buzy = true;
    assert(current->next == null);
    assert(current->prev == tail);
    assert(current_data->addr >= KHEAP_START_ADDR);
    assert(current_data->addr < KHEAP_END_ADDR);
    return (void*)current_data->addr;
}

/*
 * Api - Kernel aligned memory alloc
 */
extern void* kmalloc_a(size_t size, u_int align)
{
    void* const ptr = kmalloc(size + align);

    if (!ptr) {
        return null;
    }

    size_t const data_addr = ((size_t) ptr) + align;
    size_t mask = ~(align - 1);
    void* const aligned_ptr = (void *) (data_addr & mask);
    ((void **) aligned_ptr)[-1] = ptr; /* original address */

    return aligned_ptr;
}

/*
 * Api - Kernel free aligned memory
 */
extern void kfree_a(void* addr)
{
    void* ptr = ((void **)addr)[-1];
    kfree(ptr);
}

/*
 * Api - Kernel memory free
 */
extern void kfree(void* addr)
{
    struct slist_head_t* current = null;
    struct kheap_entry_t* current_data = null;
    struct slist_head_t* head = kheap_list.head;

    for (current = head; current != null; current = current->next) {
        current_data = (struct kheap_entry_t*)current->data;
        if (current_data->addr == (size_t)addr && current_data->is_buzy) {
            struct slist_head_t* prev = current->prev;
            struct slist_head_t* next = current->next;
            struct kheap_entry_t* prev_data = prev != null ? (struct kheap_entry_t*)prev->data : null;
            struct kheap_entry_t* next_data = next != null ? (struct kheap_entry_t*)next->data : null;

            /* free block */
            current_data->is_buzy = false;
            /* try to merge with free left sibling */
            if (prev != null && !prev_data->is_buzy) {
                prev_data->size += current_data->size;
                slist_delete_entry(&kheap_list, (struct slist_head_t*)current);
            }
            /* try to merge with free right sibling */
            if (next != null && !next_data->is_buzy) {
                current_data->size += next_data->size;
                slist_delete_entry(&kheap_list, (struct slist_head_t*)next);
            }

            return;
        }
    }

    abort(MSG_KERNEL_HEAP_FREE_INVALID_ADDR, addr);
}

/*
 * Api - Kernel heap dump
 */
extern void kheap_dump()
{
    printf("-- dump kernel heap\n");

    struct slist_head_t* current;
    struct kheap_entry_t* current_data = null;

    for (current = kheap_list.head; current != null; current = current->next) {
        current_data = (struct kheap_entry_t*)current->data;
        printf("  %X - %X bizy=%u this=%X prev=%X next=%X\n",
            current_data->addr,
            current_data->addr + current_data->size,
            current_data->is_buzy,
            current,
            current->prev,
            current->next);
    }
}

/*
 * Smoke test
 */
static void kheap_test()
{
#ifdef TEST
    /* 1[16] 2[16] 3[16] */
    void* addr1 = kmalloc(16);
    assert(addr1 == (void*)KHEAP_START_ADDR);
    void* addr2 = kmalloc(16);
    assert(addr2 == (void*)((size_t)addr1 + 16));
    void* addr3 = kmalloc(16);
    assert(addr3 == (void*)((size_t)addr2 + 16));
    /* 1[16] ![16] 3[16] */
    kfree(addr2);
    /* 1[16] 4[8] ![8] 3[16] */
    void* addr4 = kmalloc(8);
    assert(addr4 == (void*)((size_t)addr1 + 16));
    /* 1[16] 4[8] 5[8] 3[16] */
    void* addr5 = kmalloc(6);
    assert(addr5 == (void*)((size_t)addr4 + 8));
    /* 1[16] 4[8] 5[8] ![2] 3[16] 6[16] */
    void* addr6 = kmalloc(16);
    assert(addr6 == (void*)((size_t)addr3 + 16));
    /* ![16] 4[8] 5[8] ![2] 3[16] ![16] */
    kfree(addr1);
    kfree(addr6);
    /* 7[4] ![12] 4[8] 5[8] ![2] 3[16] ![16] */
    void* addr7 = kmalloc(4);
    assert(addr7 == (void*)(addr1));
    /* 7[4] 8[4] ![8] 4[8] 5[8] ![2] 3[16] ![16] */
    void* addr8 = kmalloc(4);
    assert(addr8 == (void*)((size_t)addr1 + 4));
    /* 7[4] 8[4] ![8] 4[8] 5[8] ![2] 3[16] 9[64] */
    void* addr9 = kmalloc(64);
    assert(addr9 == (void*)((size_t)addr3 + 16));

    /* ![64+16+2+8+8+8+4+4] */
    kfree(addr3);
    kfree(addr4);
    kfree(addr5);
    kfree(addr7);
    kfree(addr8);
    kfree(addr9);

    /* 10[1] ![64+16+2+8+8+8+4+4] */
    void* addr10 = kmalloc(1);
    assert(addr10 == (void*)KHEAP_START_ADDR);
    kfree(addr10);

    /* clear heap table */
    memset(kheap_blocks, 0, sizeof(struct kheap_entry_t) * KHEAP_MAX_ENTRIES);
    kheap_list.head = null;
    kheap_list.tail = null;
#endif
}

/*
 * Smoke test
 */
static void kheap_test_aligned()
{
#ifdef TEST
    /* test aligned adresses */
    void* addr1 = kmalloc_a(1024, 4096);
    assert((size_t)addr1 % 4096 == 0);

    void* addr2 = kmalloc_a(20, 4096);
    assert((size_t)addr2 % 4096 == 0);

    void* addr3 = kmalloc_a(8, 16);
    assert((size_t)addr3 % 16 == 0);

    void* addr4 = kmalloc_a(32, 32);
    assert((size_t)addr3 % 32 == 0);

    kfree_a(addr2);

    void* addr5 = kmalloc_a(64, 2048);
    assert((size_t)addr5 % 2048 == 0);
    assert((size_t)addr5 == (size_t)addr1 + 2048);

    kfree_a(addr1);
    kfree_a(addr3);
    kfree_a(addr4);
    kfree_a(addr5);


    /* clear heap table */
    memset(kheap_blocks, 0, sizeof(struct kheap_entry_t) * KHEAP_MAX_ENTRIES);
    kheap_list.head = null;
    kheap_list.tail = null;
#endif
}
