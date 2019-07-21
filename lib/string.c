#include <lib/string.h>
#include <lib/stdarg.h>

/*
 * Calculate string's length
 */
int strlen(char *s)
{
    char *original = s;

    while (*s != '\0') s++;

    return s - original;
}

/*
 * Strings' copy
 * Copies second string to first
 */
char *strcpy(register char *s1, register char *s2)
{
    char *original = s1;

    while (*s2 != '\0') *s1++ = *s2++;
    *s1 = '\0';

    return original;
}

/*
 * Compare strings
 */
int strcmp(register char *s1, register char *s2)
{
    while (1) {
        if (*s1 != *s2) return(*s1 - *s2);
        if (*s1 == '\0') return(0);
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

    while (*s1 != '\0') s1++;
    while (*s2 != '\0') *s1++ = *s2++;
    *s1 = '\0';

    return original;
}

/*
 * Extend string with attribute symbol
 */
char *strext(register char *buf, register char *str, char sym) {
    while (*str != '\0') {
        *buf++ = *str++;
        *buf++ = sym;
    }

    return buf;
}

/*
 * Integer to string
 */
char *itoa(int value, char *str, int base) {
    char *original = str;
    unsigned char digit;

    do {
        digit = value % base;
        value = value / base;
        if (digit < 10) {
            *str++ = digit | 0x30; /* number */
        } else {
            *str++ = (16 - digit) | 0x41; /* alpha */
        }
    } while (value > 0);

    if (base == 16) {
        /* hexedecimal integer */
        *str++ = 'x';
        *str++ = '0';
    } else if (base == 8) {
        /* octal integer */
        *str++ = 'o';
        *str++ = '0';
    } else if (base == 2) {
        /* binary integer */
        *str++ = 'b';
        *str++ = '0';
    }
    *str++ = '\0';

    strinv(original);

    return str;
}

/*
 * Inverse string
 */
char *strinv(char *str) {
    int i;
    unsigned int n = strlen(str);
    char buf[n+2];
    char *cur = buf;

    for (i = n - 1; i >= 0; --i) {
        *cur++ = str[i];
    }
    *cur++ = '\0';

    strcpy(str, buf);

    return str;
}

/*
 * Print to string
 */
unsigned int sprintf(char *s1, char *s2, ...) {
    unsigned int j = 0;
    char number[16];
    char *cur = s1;

    va_list list;
    va_start(list, s2);

    while (s2[j] != '\0') {
        if (s2[j] != '%') {
            /* text */
            *cur++ = s2[j++];
        } else if (s2[j] == '%') {
            /* control character */
            switch (s2[++j]) {
                case 'c':
                    /* character */
                    *cur++ = va_arg(list, char);
                    break;
                case 'u':
                    itoa(va_arg(list, unsigned int), number, 10);
                    strinv(number);
                    strcpy(cur, number);
                    cur += strlen(number);
                    break;
                case 'X':
                    itoa(va_arg(list, unsigned int), number, 16);
                    strinv(number);
                    strcpy(cur, number);
                    cur += strlen(number);
                    break;
            }
            j += 1;
        }
    }
    *cur++ = '\0';

    va_end(list);

    return ((unsigned long)cur - (unsigned long)s1);
}

/*
 * Print to limited string
 */
unsigned int snprintf(char *s1, unsigned int n, char *s2, ...) {
    unsigned int j = 0;
    char number[16];
    char *cur = s1;

    va_list list;
    va_start(list, s2);

    while (s2[j] != '\0' && j < n - 1) {
        if (s2[j] != '%') {
            /* text */
            *cur++ = s2[j++];
        } else if (s2[j] == '%') {
            /* control character */
            switch (s2[++j]) {
                case 'c':
                    /* character */
                    *cur++ = va_arg(list, char);
                    break;
                case 'u':
                    itoa(va_arg(list, unsigned int), number, 10);
                    strcpy(cur, number);
                    cur += strlen(number);
                    break;
                case 'X':
                    itoa(va_arg(list, unsigned int), number, 16);
                    strcpy(cur, number);
                    cur += strlen(number);
                    break;
            }
            j += 1;
        }
    }
    *cur++ = '\0';

    va_end(list);

    return ((unsigned long)cur - (unsigned long)s1);
}
