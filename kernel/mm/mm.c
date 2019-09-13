#include <lib/assert.h>
#include <lib/stdio.h>
#include <lib/string.h>
#include <mm/mm.h>

static size_t mm_get_addr(int ordinal);
static inline bool mm_get_bit(int ordinal);
static inline void mm_set_bit(int ordinal);
static inline void mm_clear_bit(int ordinal);
static void mm_to_coordinates(int ordinal, int* i, int* j);
static void mm_test();

static u32 bitmap[MM_BITMAP_SIZE];

/*
 * Api - memory manager init
 */
extern void mm_init()
{
    memset(&bitmap, 0, sizeof(u32) * MM_BITMAP_SIZE);
    mm_test();
}

/*
 * Api - allocate pages
 */
extern void* mm_phys_alloc_pages(u_int count)
{
    /* find free pages */
    for (int i = 0; i < MM_DYNAMIC_PAGES_COUNT; ++i) {
        bool is_found = true;
        for (int j = 0; j < count; ++j) {
            is_found = is_found && !mm_get_bit(i + j);
        }

        if (is_found) {
            /* occupy */
            for (int j = 0; j < count; ++j) {
                assert(!mm_get_bit(i + j));
                mm_set_bit(i + j);
            }

            return (void *)mm_get_addr(i);
        }
    }

    return null;
}

/*
 * Api - free page
 */
extern bool mm_phys_free_pages(void* ptr, u_int count)
{
    size_t address = (size_t)ptr;
    assert(address >= MM_AREA_START);
    assert(address % MM_PAGE_SIZE == 0);

    /* find page */
    for (int i = 0; i < MM_DYNAMIC_PAGES_COUNT; ++i) {
        size_t addr = mm_get_addr(i);
        if (addr == address) {
            /* free pages */
            for (int j = 0; j < count; ++j) {
                assert(mm_get_bit(i + j));
                mm_clear_bit(i + j);
            }

            return true;
        }
    }

    return false;
}

/*
 * Api - page bitmap dump
 */
extern void mm_dump()
{
    printf(" -- Memory manager bitmap dump\n");
    for (int i = 0; i < MM_DYNAMIC_PAGES_COUNT; ++i) {
        size_t addr = mm_get_addr(i);
        if (mm_get_bit(i)) {
            /* buzy */
            printf("!%X", addr);
        } else {
            /* free */
            printf("#%X", addr);
        }
    }
    printf("\n");
}

/*
 * Get address
 */
static size_t mm_get_addr(int ordinal)
{
    int i = 0;
    int j = 0;
    mm_to_coordinates(ordinal, &i, &j);
    return MM_AREA_START + i * sizeof(u32) * 8 * MM_PAGE_SIZE + j * MM_PAGE_SIZE;
}

/*
 * Bit operations
 */

static inline bool mm_get_bit(int ordinal)
{
    int i = 0;
    int j = 0;
    mm_to_coordinates(ordinal, &i, &j);
    u32 mask = (0x1 << j);
    return !!(bitmap[i] & mask);
}

static inline void mm_set_bit(int ordinal)
{
    int i = 0;
    int j = 0;
    mm_to_coordinates(ordinal, &i, &j);
    u32 mask = (0x1 << j);
    u32 bit = bitmap[i] & mask;
    if (bit == 0) {
        bitmap[i] |= mask;
    }
}

static inline void mm_clear_bit(int ordinal)
{
    int i = 0;
    int j = 0;
    mm_to_coordinates(ordinal, &i, &j);
    u32 mask = (0x1 << j);
    u32 bit = bitmap[i] & mask;
    if (bit != 0) {
        bitmap[i] ^= mask;
    }
}

/*
 * Get page coordinates
 */
static void mm_to_coordinates(int ordinal, int* i, int* j)
{
    *i = ordinal / (sizeof(u32) * 8);
    *j = ordinal % (sizeof(u32) * 8);
}

/*
 * Smoke test
 */
static void mm_test()
{
#ifdef TEST
    // alloc 3 pages (1, 1, 1)
    void* p1 = mm_phys_alloc_pages(1);
    assert(mm_get_bit(0));
    void* p2 = mm_phys_alloc_pages(1);
    assert(mm_get_bit(1));
    void* p3 = mm_phys_alloc_pages(1);
    assert(mm_get_bit(2));
    assert((size_t)p1 < (size_t)p2);
    assert((size_t)p2 < (size_t)p3);
    assert((size_t)p2 == (size_t)p1 + MM_PAGE_SIZE);
    // free 2th page (1, 0, 1)
    assert(mm_phys_free_pages(p2, 1));
    assert(mm_get_bit(0));
    assert(!mm_get_bit(1));
    assert(mm_get_bit(2));
    // alloc 1 page (1, 1, 1)
    void* p4 = mm_phys_alloc_pages(1);
    assert(mm_get_bit(1));
    // alloc 1 page (1, 1, 1, 1)
    void* p5 = mm_phys_alloc_pages(1);
    assert(mm_get_bit(3));
    // alloc 32 pages
    for (int i = 0; i < 32; ++i) {
        mm_phys_alloc_pages(1);
    }
    for (int i = 0; i < 32; ++i) {
        assert(mm_get_bit(i));
    }
    assert(mm_get_bit(32));
    assert(mm_get_bit(33));
    assert(mm_get_bit(34));
    assert(mm_get_bit(35));
    // free pages
    assert(mm_phys_free_pages(p1, 1));
    assert(mm_phys_free_pages(p3, 1));
    assert(mm_phys_free_pages(p4, 1));
    assert(mm_phys_free_pages(p5, 1));
    // clear
    memset(&bitmap, 0, sizeof(u32) * MM_BITMAP_SIZE);
#endif
}
