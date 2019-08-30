#pragma once

#include <lib/assembly.h>
#include <lib/stdtypes.h>

#ifdef KERNEL
#include <utils/kassert.h>
#define assert(expr) kassert(__FILE__, __LINE__, expr)
#define unreachable() kunreachable(__FILE__, __LINE__)
#else
#define assert(expr) uassert(__FILE__, __LINE__, expr)
#define unreachable() uunreachable(__FILE__, __LINE__)
#endif

/*
 * Api
 */
extern void uassert(const char* file, u_int line, bool expr) __attribute__ ((noreturn));
extern void uunreachable(const char* file, u_int line) __attribute__ ((noreturn));
