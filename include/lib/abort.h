#pragma once

#include <lib/stdint.h>
#include <lib/stdbool.h>
#include <lib/stdarg.h>

#ifdef KERNEL
/* for kernel */
extern void kpanic(char* message, ...);
#else
/* for user applications */
#endif
