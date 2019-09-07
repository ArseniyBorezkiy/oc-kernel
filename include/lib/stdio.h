#pragma once

#include <lib/stdint.h>
#include <lib/stdbool.h>

#ifdef KERNEL
/* for kernel */
#define printf(msg, ...) kprintf(msg, ##__VA_ARGS__)
#define vprintf(msg, list) kvprintf(msg, list)
#define clear() kclear()
extern void kclear();
extern void kprintf(const char* format, ...);
extern void kvprintf(const char* format, va_list list);
extern void kmode(bool is_early);
extern void klog(char* buf, u_int n);
#else
/* for user applications */
#endif
