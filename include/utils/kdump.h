#pragma once

#include <lib/stdtypes.h>

/*
 * Api
 */
extern void kdump_stack(size_t *addr);
extern void kdump_multiboot(struct mutliboot_t *header);
