#include <lib/string.h>

/*
 * Api - Strings' copy
 * Copies second string to first
 */
extern char* strcpy(char* s1, const char* s2)
{
    while (*s2 != '\0')
        *s1++ = *s2++;
    *s1++ = '\0';

    return s1;
}

/*
 * Api - Memory set
 * Fill buffer with value
 */
extern char* memset(void* buff, const char value, unsigned int size)
{
    for (int i = 0; i < size; ++i) {
        *(char *)buff++ = value;
    }

    return buff;
}

/*
 * Api - String extend
 * Extend string with attribute symbol
 */
extern char* strext(char* buf, const char* str, char sym)
{
    while (*str != '\0') {
        *buf++ = *str++;
        *buf++ = sym;
    }

    return buf;
}
