#pragma once

/*
 * POSIX string api
 */

int strlen(char *s);
char *strcpy(register char *s1, register char *s2);
int strcmp(register char *s1, register char *s2);
char *strcat(register char *s1, register char *s2);
char *itoa(int value, char *str, int base);
unsigned int sprintf(char *buf, char *str, ...);
