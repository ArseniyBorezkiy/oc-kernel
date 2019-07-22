#include <lib/math.h>

/*
 * Api - Minimun of two numbers
 */

extern unsigned int min(unsigned int a, unsigned int b) {
    if (a <= b) {
        return a;
    }

    return b;
}

/*
 * Api - Maximum of two numbers
 */

extern unsigned int max(unsigned int a, unsigned int b) {
    if (a > b) {
        return a;
    }

    return b;
}
