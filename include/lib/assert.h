#pragma once

#include <lib/assembly.h>

#ifdef KERNEL
#include <utils/kassert.h>
#define assert(expr) kassert(__FILE__, __LINE__, expr)
#define unreachable() kunreachable(__FILE__, __LINE__)
#else
extern void uassert(const char *file, u_int line, bool expr);
extern void uunreachable(const char *file, u_int line);
#define assert(expr) uassert(__FILE__, __LINE__, expr)
#define unreachable() uunreachable(__FILE__, __LINE__)
#endif
