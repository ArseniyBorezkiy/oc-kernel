#include <lib/math.h>

/*
 * Minimun of two numbers
 */

unsigned int min(unsigned int a, unsigned int b) {
    if (a <= b) {
        return a;
    }

    return b;
}

/*
 * Maximum of two numbers
 */

unsigned int max(unsigned int a, unsigned int b) {
    if (a > b) {
        return a;
    }

    return b;
}
