#pragma once

#include <lib/assembly.h>

struct io_buf_t
{
  int fd;      /* file descriptor */
  char *base;  /* buffer beginning */
  char *ptr;   /* position in buffer */
  bool is_eof; /* whether end of file */
};

#define FILE struct io_buf_t

#ifdef KERNEL
#define printf(msg, ...) kprintf(msg, ##__VA_ARGS__)
#define vprintf(msg, list) kvprintf(msg, list)
#define clear() kclear()
#else
#define printf(msg, ...) uprintf(msg, ##__VA_ARGS__)
#define vprintf(msg, list) uvprintf(msg, list)
#define clear() uclear()
#endif

/*
 * Api
 */
extern void puts(const char *str);
extern void putc(char ch);
extern void uclear();
