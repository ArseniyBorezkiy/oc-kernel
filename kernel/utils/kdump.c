#include <boot/multiboot.h>
#include <utils/kdump.h>
#include <utils/kprint.h>
#include <lib/stdtypes.h>

/*
 * Api - Stack dump
 */
extern void kdump_stack(size_t *addr) {
  size_t base = (size_t)addr;

  kprint("-- stack dump: %X\n", base);
  kprint("  %X : %X\n", base, *(u_int*)base);
  kprint("  %X : %X\n", base + 4, *(u_int*)(base + 4));
  kprint("  %X : %X\n", base + 8, *(u_int*)(base + 8));
}

/*
 * Api - Multiboot header dump
 */
extern void kdump_multiboot(struct multiboot_t *header) {
  kprint("-- multiboot header dump: %X\n", (size_t)header);
  kprint("  mem_lower : %X Kb\n", header->mem_lower);
  kprint("  mem_upper : %X Kb\n", header->mem_upper);
}
