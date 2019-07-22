#pragma once

#include <types.h>

/*
 * Api
 */
extern void kclear(); /* clear kernel screen */
extern void kprint(const char *str, ...); /* kernel print */
extern void stack_dump(size_t *addr);
