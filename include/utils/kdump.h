#pragma once

#include <multiboot.h>
#include <lib/stdtypes.h>
#include <vfs/elf.h>

/*
 * Api
 */
extern void kdump_stack(size_t* esp);
extern void kdump_multiboot(struct multiboot_t* header);
extern void kdump_registers();
extern void kdump_eflags();
extern void kdump_memory_areas();
