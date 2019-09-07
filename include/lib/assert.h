#pragma once

#include <lib/stdint.h>
#include <lib/stdbool.h>

#ifdef KERNEL
/* for kernel */
#define assert(expr) kassert(__FILE__, __LINE__, expr)
#define unreachable() kunreachable(__FILE__, __LINE__)
extern void kassert(const char* file, u_int line, bool expr);
extern void kunreachable(const char* file, u_int line);
#else
/* for user applications */
#endif
