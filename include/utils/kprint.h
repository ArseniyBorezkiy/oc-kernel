#pragma once

#include <types.h>

/*
 * Api
 */
extern void kclear(); /* clear kernel screen */
extern void kprint(const char *str, ...); /* kernel print */
extern void kscroll(u_int n); /* scroll console up */
