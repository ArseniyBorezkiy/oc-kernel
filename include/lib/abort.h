#pragma once

#include <lib/stdint.h>
#include <lib/stdbool.h>
#include <lib/stdarg.h>

#ifdef KERNEL
/* for kernel */
#define abort(msg, ...) kpanic(msg, ##__VA_ARGS__)
extern void kpanic(char* message, ...);
#else
/* for user applications */
#endif
