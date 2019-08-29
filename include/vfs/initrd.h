#pragma once

#include <lib/stdtypes.h>
#include <boot/multiboot.h>

#define INITRD_MAX_FILES 8

extern struct initrd_node_t
{
  char name[8]; /* file name */
  size_t base;  /* file begin */
  size_t size;  /* file size */
};

extern struct initrd_fs_t
{
  struct initrd_node_t *first[INITRD_MAX_FILES]; /* first node */
  int count;                                     /* nodes count */
};

/*
 * Api
 */
extern void initrd_autorun(struct mod_addr_t *mods_addr, int mods_count);
