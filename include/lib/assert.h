#pragma once

#include <lib/assembly.h>

#ifdef KERNEL
#include <utils/kassert.h>
#define assert(expr) kassert(__FILE__, __LINE__, expr)
#else
#define assert(expr) uassert(expr)
#endif
