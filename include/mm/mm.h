#pragma once

#include <lib/stdtypes.h>
#include <vfs/elf.h>

#define MM_PAGE_SIZE 4096 /* 4 KB */
#define MM_BITMAP_SIZE 8 /* 1 MB memory area */
#define MM_AREA_START 0x300000 /* dynamic pages area start */
#define MM_AREA_END 0x400000 /* dynamic pages area end */
#define MM_DYNAMIC_PAGES_COUNT (MM_BITMAP_SIZE * sizeof(u32) * 8) /* how many dynamic pages */

/*
 * Api
 */
extern void mm_init();
extern void* mm_phys_alloc_pages(u_int count);
extern bool mm_phys_free_pages(void* ptr, u_int count);
extern void mm_dump();
