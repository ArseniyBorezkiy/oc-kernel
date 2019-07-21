#pragma once

typedef unsigned long * va_list;

#define va_start(l, a) (l = (void *)((unsigned long)&a) + sizeof(a))
#define va_end(l) (l = (void *) 0)
#define va_arg(l, s) (*(s*)(l++))
