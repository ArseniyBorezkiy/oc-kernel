#pragma once

#include <lib/stdtypes.h>

#define SYSLOG_SIZE VIDEO_SCREEN_SIZE

/*
 * Api
 */
extern void kmode(bool is_early);                   /* set print mode */
extern void kclear();                               /* clear kernel screen */
extern void kprintf(const char *str, ...);           /* kernel print */
extern void kvprintf(const char *str, va_list list); /* kernel print */
