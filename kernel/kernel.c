#include <messages.h>
#include <arch/memory.h>
#include <arch/idt.h>
#include <arch/pic.h>
#include <utils/kprint.h>

/*
 * Kernel entry point
 */
void kernel_start(void)
{
	kclear();
    idt_init();
    pic_enable();
	kprint(MSG_KERNEL_START);

	while(1);
}

