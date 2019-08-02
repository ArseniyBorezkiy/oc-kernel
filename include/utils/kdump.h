#pragma once

#include <lib/stdtypes.h>
#include <boot/multiboot.h>
#include <vfs/elf.h>

/*
 * Api
 */
extern void kdump_stack(size_t *addr);
extern void kdump_multiboot(struct mutliboot_t *header);
extern void kdump_elf(struct elf_header_t *header);
