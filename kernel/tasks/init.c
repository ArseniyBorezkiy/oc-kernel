#include <arch/reg.h>
#include <arch/idt.h>
#include <tasks/init.h>
#include <utils/kprint.h>
#include <utils/kdump.h>
#include <lib/time.h>
#include <lib/stdtypes.h>
#include <messages.h>

/*
 * Api - Init task
 */
extern void task_init_main()
{
    while (1)
    {
        kprint(MSG_TASK_INIT_LAUNCHED, asm_get_esp(), asm_get_eflags());
        delay(1);
    }
}
