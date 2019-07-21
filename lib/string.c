#include <lib/string.h>
#include <lib/stdarg.h>

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

/*
 * Integer to string
 */
char *itoa(int value, char *str, int base) {
    unsigned char digit;

    if (base == 16) {
        /* hexedecimal integer */
        *str++ = '0';
        *str++ = 'x';
    } else if (base == 8) {
        /* octal integer */
        *str++ = '0';
        *str++ = 'o';
    } else if (base == 2) {
        /* binary integer */
        *str++ = '0';
        *str++ = 'b';
    }

    do {
        digit = value % base;
        value = value / base;
        if (digit < 10) {
            *str++ = digit | 0x30; /* number */
        } else {
            *str++ = (16 - digit) | 0x41; /* alpha */
        }
    } while (value > 0);
    *str = 0; /* end of text */

    return str;
}

/*
 * Spring print
 */
unsigned int sprintf(char *buf, char *str, ...) {
    unsigned int j = 0;
    char number[16];
    char *cur = buf;

    va_list list;
    va_start(list, str);

    while (str[j] != '\0') {
        if (str[j] != '%') {
            /* text */
            *cur++ = str[j++];
        } else if (str[j] == '%') {
            /* control character */
            switch (str[++j]) {
                case 'c':
                    /* character */
                    *cur++ = va_arg(list, char);
                    break;
                case 'u':
                    itoa(va_arg(list, unsigned int), &number[0], 10);
                    strcpy(cur, &number[0]);
                    cur += strlen(&number[0]);
                    break;
                case 'X':
                    itoa(va_arg(list, unsigned int), &number[0], 16);
                    strcpy(cur, &number[0]);
                    cur += strlen(&number[0]);
                    break;
            }
            j += 1;
        }
    }
    *cur++ = 0; /* terminator */

    va_end(list);

    return ((unsigned long)cur - (unsigned long)buf);
}
