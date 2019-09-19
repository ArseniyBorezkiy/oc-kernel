#include <lib/math.h>

/*
 * Api - Minimun of two numbers
 */

extern u_int min(u_int a, u_int b)
{
    if (a <= b) {
        return a;
    }

    return b;
}

/*
 * Api - Maximum of two numbers
 */

extern u_int max(u_int a, u_int b)
{
    if (a > b) {
        return a;
    }

    return b;
}
