#pragma once

#include <lib/stdtypes.h>

#define SCREEN_WIDTH (80 * 2)
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)
#define VIDEO_MEMORY_ADDR 0xb8000
#define VIDEO_MEMORY_ATTR 0x07

extern char* syslog_ptr;

/*
 * Api
 */
extern void init_video(); /* init module */
extern void init_syslog(); /* turn on print to system log */
extern void kclear(); /* clear kernel screen */
extern void kprint(const char *str, ...); /* kernel print */
extern void kvprint(const char *str, va_list list); /* kernel print */
extern void kscroll(u_int n); /* scroll console up */
