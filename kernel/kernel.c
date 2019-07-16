#include <messages.h>
#include <arch/memory.h>
#include <arch/interrupt.h>
#include <utils/kprint.h>

/*
 * Kernel entry point
 */
void kernel_start(void)
{
	kclear();
    idt_init();
    keyboard_init();
	kprint(MSG_KERNEL_START);

	while(1);
}

