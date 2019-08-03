#include <arch/reg.h>
#include <utils/kassert.h>
#include <utils/kpanic.h>
#include <kernel.h>
#include <messages.h>

/*
 * Kernel boolean assertion
 */
extern void kassert(u_int line, u_int column, bool expr) {
    if (!expr) {
        kprint(MSG_KERNEL_LINE_COLUMN, line, column);
        kpanic("assertion falied");
    }
}

/*
 * Kernel boolean assertion
 */
extern void kassert_kernel_stack(u_int line, u_int column) {
    size_t kernel_stack_addr = (size_t)kernel_stack;
    u32 esp = asm_get_esp();
    if (esp > kernel_stack_addr || esp < kernel_stack_addr + KERNEL_STACK_SIZE) {
        kprint(MSG_KERNEL_LINE_COLUMN, line, column);
        kpanic("kernel stack expected, but esp = %X", esp);
    }
}
