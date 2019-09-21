#include <arch/arch.h>
#include <kernel.h>
#include <messages.h>
#include <utils/kassert.h>
#include <utils/kpanic.h>
#include <utils/kprint.h>

/*
 * Kernel boolean assertion
 */
extern void kassert(const char* file, u_int line, bool expr)
{
    if (!expr) {
        kpanic(MSG_KERNEL_FILE_LINE, file, line);
    }
}

/* 
 * Kernel code unreachable
 */
extern void kunreachable(const char* file, u_int line)
{
    kprintf(MSG_KERNEL_CODE_UNREACHABLE);
    kpanic(MSG_KERNEL_FILE_LINE, file, line);
}

/*
 * Kernel boolean assertion
 */
extern void kassert_kernel_stack(const char* file, u_int line)
{
    size_t kernel_stack_addr = (size_t)kernel_stack;
    u32 esp = asm_get_esp();
    if (esp > kernel_stack_addr || esp < kernel_stack_addr + KERNEL_STACK_SIZE) {
        kprintf(MSG_KERNEL_STACK_EXPECTED, esp);
        kpanic(MSG_KERNEL_FILE_LINE, file, line);
    }
}
