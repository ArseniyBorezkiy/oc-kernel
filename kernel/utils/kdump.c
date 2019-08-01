#include <utils/kdump.h>
#include <utils/kprint.h>
#include <types.h>

/*
 * Api - Stack dump
 */
extern void kdump_stack(size_t *addr) {
  u_int base = (size_t)addr;

  kprint("-- stack dump: %X\n", base);
  kprint("  %X : %X\n", base, *(u_int*)base);
  kprint("  %X : %X\n", base + 4, *(u_int*)(base + 4));
  kprint("  %X : %X\n", base + 8, *(u_int*)(base + 8));
}
