#include <lib/string.h>

/*
 * Calculate string's length
 */
int strlen(char *s)
{
    char *original = s;

    while (*s != 0) s++;

    return s - original;
}

/*
 * Strings' copy
 * Copies second string to first
 */
char *strcpy(register char *s1, register char *s2)
{
    char *original = s1;

    while (*s2 != 0) *s1++ = *s2++;
    *s1 = 0;

    return original;
}

/*
 * Compare strings
 */
int strcmp(register char *s1, register char *s2)
{
    while (1) {
        if (*s1 != *s2) return(*s1 - *s2);
        if (*s1 == 0) return(0);
        s1++;
        s2++;
    }
}

/*
 * Strings' concatenation
 * Append second to the end of the first
 */
char *strcat(register char *s1, register char *s2)
{
    char *original = s1;

    while (*s1 != 0) s1++;
    while (*s2 != 0) *s1++ = *s2++;
    *s1 = 0;

    return original;
}
