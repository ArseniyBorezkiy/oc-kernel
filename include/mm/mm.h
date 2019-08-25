#pragma once

#include <vfs/elf.h>

#define MM_PAGE_SIZE 4096 /* 4 KB */
#define MM_BITMAP_SIZE 8 /* 1 MB memory area */
#define MM_AREA_START 0x300000
#define MM_AREA_END 0x400000

/*
 * Api
 */
extern void mm_init();
extern void *mm_alloc_page();
extern bool mm_free_page(void *ptr);
extern void mm_dump();
