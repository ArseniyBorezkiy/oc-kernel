#pragma once

#include <types.h>

/*
 * POSIX string api
 */

extern int strlen(char *s);
extern char *strcpy(register char *s1, register char *s2);
extern void *memcpy(register void *buf1, register void *buf2, u_int bytes);
extern int strcmp(register char *s1, register char *s2);
extern char *strcat(register char *s1, register char *s2);
extern char *strext(register char *buf, register char *str, char sym);
extern char *itoa(int value, char *str, int base);
extern char *strinv(char *str);
extern unsigned int sprintf(char *s1, char *s2, ...);
extern unsigned int snprintf(char *s1, u_int n, char *s2, ...);
