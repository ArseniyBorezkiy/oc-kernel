#include <mm/mm.h>
#include <lib/assert.h>
#include <lib/string.h>
#include <lib/stdio.h>

static size_t mm_get_addr(int i, int j);
static inline bool mm_get_bit(int i, int j);
static inline void mm_set_bit(int i, int j);
static inline void mm_clear_bit(int i, int j);
static void mm_test();

static u32 bitmap[MM_BITMAP_SIZE];

/*
 * Api - memory manager init
 */
extern void mm_init() {
  memset(&bitmap, 0, sizeof(u32) * MM_BITMAP_SIZE);
  mm_test();
}

/*
 * Api - allocate page
 */
extern void *mm_alloc_page() {
  // find free page
  for (int i = 0; i < MM_BITMAP_SIZE; ++i) {
    for (int j = 0; j < 32; ++j) {
      if (!mm_get_bit(i, j)) {
        // occupy page
        mm_set_bit(i, j);
        size_t addr = MM_AREA_START + i * 32 * MM_PAGE_SIZE + j * MM_PAGE_SIZE;
        return (void *)addr;
      }
    }
  }

  return null;
}

/*
 * Api - free page
 */
extern bool mm_free_page(void *ptr) {
  size_t address = (size_t)ptr;
  assert(address >= MM_AREA_START);
  assert(address % MM_PAGE_SIZE == 0);

  // find page
  for (int i = 0; i < MM_BITMAP_SIZE; ++i) {
    for (int j = 0; j < 32; ++j) {
      if (mm_get_bit(i, j)) {
        size_t addr = MM_AREA_START + i * 32 * MM_PAGE_SIZE + j * MM_PAGE_SIZE;
        if (addr == address) {
          mm_clear_bit(i, j);
          return true;
        }
      }
    }
  }

  return false;
}

/*
 * Api - page bitmap dump
 */
extern void mm_dump() {
  printf(" -- Memory manager bitmap dump\n");
  for (int i = 0; i < MM_BITMAP_SIZE; ++i) {
    printf("  %X  ", mm_get_addr(i, 0));
    for (int j = 0; j < 32; ++j) {
      printf(" %u", mm_get_bit(i, j));
    }
    printf("\n");
  }
}

/*
 * Get address
 */
static size_t mm_get_addr(int i, int j) {
  return MM_AREA_START + i * 32 * MM_PAGE_SIZE + j * MM_PAGE_SIZE;
}

/*
 * Bit operations
 */

static inline bool mm_get_bit(int i, int j) {
  u32 mask = (0x1 << j);
  return !!(bitmap[i] & mask);
}

static inline void mm_set_bit(int i, int j) {
  u32 mask = (0x1 << j);
  u32 bit = bitmap[i] & mask;
  if (bit == 0) {
    bitmap[i] |= mask;
  }
}

static inline void mm_clear_bit(int i, int j) {
  u32 mask = (0x1 << j);
  u32 bit = bitmap[i] & mask;
  if (bit != 0) {
    bitmap[i] ^= mask;
  }
}

/*
 * Smoke test
 */
static void mm_test() {
#ifdef TEST
  // alloc 3 pages (1, 1, 1)
  void *p1 = mm_alloc_page();
  assert(mm_get_bit(0, 0));
  void *p2 = mm_alloc_page();
  assert(mm_get_bit(0, 1));
  void *p3 = mm_alloc_page();
  assert(mm_get_bit(0, 2));
  assert((size_t)p1 < (size_t)p2);
  assert((size_t)p2 < (size_t)p3);
  assert((size_t)p2 == (size_t)p1 + MM_PAGE_SIZE);
  // free 2th page (1, 0, 1)
  assert(mm_free_page(p2));
  assert(mm_get_bit(0, 0));
  assert(!mm_get_bit(0, 1));
  assert(mm_get_bit(0, 2));
  // alloc 1 page (1, 1, 1)
  void *p4 = mm_alloc_page();
  assert(mm_get_bit(0, 1));
  // alloc 1 page (1, 1, 1, 1)
  void *p5 = mm_alloc_page();
  assert(mm_get_bit(0, 3));
  // alloc 32 pages
  for (int i = 0; i < 32; ++i) {
    mm_alloc_page();
  }
  for (int i = 0; i < 32; ++i) {
    assert(mm_get_bit(0, i));
  }
  for (int i = 0; i < 4; ++i) {
    assert(mm_get_bit(1, i));
  }
  // free pages
  assert(mm_free_page(p1));
  assert(mm_free_page(p3));
  assert(mm_free_page(p4));
  assert(mm_free_page(p5));
  // clear
  memset(&bitmap, 0, sizeof(u32) * MM_BITMAP_SIZE);
#endif
}
