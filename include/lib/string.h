#pragma once

#include <types.h>

/*
 * POSIX string api
 */

int strlen(char *s);
char *strcpy(register char *s1, register char *s2);
int strcmp(register char *s1, register char *s2);
char *strcat(register char *s1, register char *s2);
char *strext(register char *buf, register char *str, char sym);
char *itoa(int value, char *str, int base);
char *strinv(char *str);
unsigned int sprintf(char *s1, char *s2, ...);
unsigned int snprintf(char *s1, u_int n, char *s2, ...);
