#pragma once

#include <lib/stdtypes.h>

/*
 * Api
 */
extern void kclear(); /* clear kernel screen */
extern void kprint(const char *str, ...); /* kernel print */
extern void kvprint(const char *str, va_list list); /* kernel print */
extern void kscroll(u_int n); /* scroll console up */
