#include <lib/stdtypes.h>
#include <lib/time.h>

/*
 * Api - Delay
 */

extern void delay(u_int sec)
{
    u_int cycles = sec * 100000000;
    for (int i = 0; i < cycles; ++i) {
        /* nop */
    }
}
