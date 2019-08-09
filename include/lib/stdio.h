#pragma once

struct io_buf_t
{
  int fd;      /* file descriptor */
  char *base;  /* buffer beginning */
  char *ptr;   /* position in buffer */
  bool is_eof; /* whether end of file */
};

#define FILE struct io_buf_t

/*
 * Api
 */
extern void puts(const char *str);
extern void putc(char ch);
extern void clear();
