#include <lib/stdtypes.h>
#include <lib/string.h>

/*
 * Api - Calculate string's length
 */
extern int strlen(const char *s)
{
    const char *original = s;

    while (*s != '\0')
        s++;

    return s - original;
}

/*
 * Api - Strings' copy
 * Copies second string to first
 */
extern char *strcpy(char *s1, const char *s2)
{
    char *original = s1;

    while (*s2 != '\0')
        *s1++ = *s2++;
    *s1 = '\0';

    return original;
}

/*
 * Api - Memory copy
 * Copies second buffer to first buffer
 */
extern void *memcpy(void *buf1, const void *buf2, u_int bytes)
{
    u8 *buf_dst = buf1;
    const u8 *buf_src = buf2;

    for (int i = 0; i < bytes; ++i)
    {
        *buf_dst++ = *buf_src++;
    }

    return buf_dst;
}

/*
 * Api - Memory set
 * Fills buffer with value
 */
extern void *memset(void *buf, u8 value, u_int bytes)
{
    u8 *buf_dst = buf;

    for (int i = 0; i < bytes; ++i)
    {
        *buf_dst++ = (u8)value;
    }

    return buf;
}

/*
 * Api - Compare strings
 */
extern int strcmp(const char *s1, const char *s2)
{
    while (1)
    {
        if (*s1 != *s2)
            return (*s1 - *s2);
        if (*s1 == '\0')
            return (0);
        s1++;
        s2++;
    }
}

/*
 * Api - Strings' concatenation
 * Append second to the end of the first
 */
extern char *strcat(char *s1, const char *s2)
{
    char *original = s1;

    while (*s1 != '\0')
        s1++;
    while (*s2 != '\0')
        *s1++ = *s2++;
    *s1 = '\0';

    return original;
}

/*
 * Api - Extend string with attribute symbol
 */
extern char *strext(char *buf, const char *str, char sym)
{
    while (*str != '\0')
    {
        *buf++ = *str++;
        *buf++ = sym;
    }

    return buf;
}

/*
 * Api - Extend memory with attribute symbol
 */
extern char *memext(void *buff_dst, u_int n, const void *buff_src, char sym)
{
    u8 *buff_dst_ptr = buff_dst;
    u8 *buff_src_ptr = (u8 *)buff_src;

    for (int i = 0; i < n; ++i)
    {
        *buff_dst_ptr++ = *buff_src_ptr++;
        *buff_dst_ptr++ = sym;
    }

    return buff_dst;
}

/*
 * Api - Integer to string
 */
extern char *itoa(int value, char *str, int base)
{
    char *original = str;
    char digit;

    do
    {
        digit = value % base;
        value = value / base;
        if (digit < 10)
        {
            *str++ = digit | 0x30; /* number */
        }
        else
        {
            *str++ = ((digit - 10) | 0x40) + 1; /* alpha */
        }
    } while (value > 0);

    if (base == 16)
    {
        /* hexedecimal integer */
        *str++ = 'x';
        *str++ = '0';
    }
    else if (base == 8)
    {
        /* octal integer */
        *str++ = 'o';
        *str++ = '0';
    }
    else if (base == 2)
    {
        /* binary integer */
        *str++ = 'b';
        *str++ = '0';
    }
    *str++ = '\0';

    strinv(original);

    return str;
}

/*
 * Api - Inverse string
 */
extern char *strinv(char *str)
{
    int i;
    u_int n = strlen(str);
    char buf[n + 2];
    char *cur = buf;

    for (i = n - 1; i >= 0; --i)
    {
        *cur++ = str[i];
    }
    *cur++ = '\0';

    strcpy(str, buf);

    return str;
}

/*
 * Api - Print to string
 */
extern unsigned int sprintf(char *s1, const char *s2, ...)
{
    va_list list;
    va_start(list, s2);

    return vsprintf(s1, s2, list);
}

/*
 * Api - Print to limited string
 */
extern unsigned int snprintf(char *s1, unsigned int n, const char *s2, ...)
{
    va_list list;
    va_start(list, s2);

    return vsnprintf(s1, n, s2, list);
}

/*
 * Api - Print to string
 */
extern unsigned int vsprintf(char *s1, const char *s2, va_list list)
{
    return vsnprintf(s1, 4 * 1024, s2, list);
}

/*
 * Api - Print to limited string
 */
extern unsigned int vsnprintf(char *s1, unsigned int n, const char *s2, va_list list)
{
    u_int j = 0;
    size_t count = 0;
    char number[16];
    char *cur = s1;
    char *str;

    while (s2[j] != '\0' && j < n)
    {
        if (s2[j] != '%')
        {
            /* text */
            *cur++ = s2[j++];
        }
        else if (s2[j] == '%')
        {
            /* control character */
            switch (s2[++j])
            {
            case 'c':
                /* character */
                *cur++ = va_arg(list, char);
                break;
            case 'u':
                /* unsigned decimal */
                itoa(va_arg(list, u_int), number, 10);
                strcpy(cur, number);
                cur += strlen(number);
                break;
            case 'X':
                /* unsigned hexedecimal */
                itoa(va_arg(list, u_int), number, 16);
                strcpy(cur, number);
                cur += strlen(number);
                break;
            case 's':
                /* string */
                str = va_arg(list, char *);
                strcpy(cur, str);
                cur += strlen(str);
                break;
            }
            j += 1;
        }
    }
    
    count = ((size_t)cur - (size_t)s1);
    *cur++ = '\0';

    va_end(list);

    return count;
}
