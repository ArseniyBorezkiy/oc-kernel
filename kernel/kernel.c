#include <messages.h>
#include <arch/memory.h>
#include <utils/kprint.h>

/*
 * Kernel entry point
 */
void kernel_start(void)
{
	kclear();
	kprint(MSG_KERNEL_START);

	return;
}

