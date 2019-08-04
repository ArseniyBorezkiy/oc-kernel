#include <arch/reg.h>
#include <utils/kpanic.h>
#include <utils/kassert.h>
#include <kernel.h>
#include <messages.h>

/*
 * Kernel boolean assertion
 */
extern void kassert(const char * file, u_int line, bool expr) {
    if (!expr) {
        kprint(file);
        kprint(MSG_KERNEL_LINE, line);
        kpanic("assertion falied");
    }
}

/*
 * Kernel boolean assertion
 */
extern void kassert_kernel_stack(const char * file, u_int line) {
    size_t kernel_stack_addr = (size_t)kernel_stack;
    u32 esp = asm_get_esp();
    if (esp > kernel_stack_addr || esp < kernel_stack_addr + KERNEL_STACK_SIZE) {
        kprint(file);
        kprint(MSG_KERNEL_LINE, line);
        kpanic("kernel stack expected, but esp = %X", esp);
    }
}
