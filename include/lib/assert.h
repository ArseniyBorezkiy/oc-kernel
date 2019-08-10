#pragma once

#include <lib/assembly.h>

#ifdef KERNEL
#include <utils/kassert.h>
#define assert(expr) kassert(__FILE__, __LINE__, expr)
#define unreachable() kunreachable(__FILE__, __LINE__)
#else
#define assert(expr) uassert(expr)
#define unreachable uunreachable(__FILE__, __LINE__)
#endif
