#pragma once

#include <lib/stdtypes.h>
#include <boot/multiboot.h>

/*
 * Api
 */
extern void initrd_autorun(struct mod_addr_t *mods_addr, int mods_count);
