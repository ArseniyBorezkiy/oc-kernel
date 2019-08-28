#pragma once

#include <lib/stdtypes.h>
#include <lib/assembly.h>

struct io_buf_t
{
  int fd;      /* file descriptor */
  char *base;  /* buffer beginning */
  char *ptr;   /* position in buffer */
  bool is_eof; /* whether end of file */
  void *file;  /* file definition */
};

#define FILE struct io_buf_t
#define IOCTL_INIT 1
#define IOCTL_FLUSH 2
#define IOCTL_CLEAR 3

#ifdef KERNEL
#define printf(msg, ...) kprintf(msg, ##__VA_ARGS__)
#define vprintf(msg, list) kvprintf(msg, list)
#define clear() kclear()
#else
#define printf(msg, ...) uprintf(msg, ##__VA_ARGS__)
#define vprintf(msg, list) uvprintf(msg, list)
#define clear() uclear()
#endif

extern FILE *stdin;
extern FILE *stdout;

/*
 * Api
 */
extern void puts(const char *str);
extern void putc(char ch);
extern void uclear();
extern void uprintf(char *format, ...);
extern void unprintf(char *format, u_int n, ...);
extern void uvprintf(const char *format, va_list list);
extern void uvnprintf(const char *format, u_int n, va_list list);
extern void flush();
extern void fputs(FILE *file, const char *str);
extern void fputc(FILE *file, char ch);
extern void fflush(FILE *file);
extern void fuclear(FILE *file);
