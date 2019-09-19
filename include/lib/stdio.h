#pragma once

#include <lib/assembly.h>
#include <lib/stdtypes.h>

struct io_buf_t {
    int fd; /* file descriptor */
    char* base; /* buffer beginning */
    char* ptr; /* position in buffer */
    bool is_eof; /* whether end of file */
    bool is_eol; /* whether end of line */
    void* file; /* file definition */
};

#define FILE struct io_buf_t
#define IOCTL_INIT 1
#define IOCTL_FLUSH 2
#define IOCTL_CLEAR 3
#define IOCTL_READ_MODE_LINE 4
#define IOCTL_READ_MODE_ECHO 5

#ifdef KERNEL
#define printf(msg, ...) kprintf(msg, ##__VA_ARGS__)
#define vprintf(msg, list) kvprintf(msg, list)
#define clear() kclear()
#else
#define puts(str) uputs(str)
#define putc(str) uputc(str)
#define clear() uclear()
#define flush() uflush()
#define printf(msg, ...) uprintf(msg, ##__VA_ARGS__)
#define scanf(msg, ...) uscanf(msg, ##__VA_ARGS__)
#define vprintf(msg, list) uvprintf(msg, list)
#endif

extern FILE* stdin;
extern FILE* stdout;

#define MOD_R 1
#define MOD_W 2

/*
 * Api
 */
extern void stdio_init();
extern void stdio_deinit();

extern void uputs(const char* str);
extern void uputc(char ch);
extern void uclear();
extern void uflush();
extern void uprintf(const char* format, ...);
extern void unprintf(const char* format, u_int n, ...);
extern void uvprintf(const char* format, va_list list);
extern void uvnprintf(const char* format, u_int n, va_list list);
extern void uscanf(char* buff, ...);

extern FILE* fopen(const char* name, int mod_rw);
extern void fclose(FILE* file);
extern u_int fread(FILE* file, char* buff, u_int size);
extern void fwrite(FILE* file, const char* data, u_int size);
extern void fputs(FILE* file, const char* str);
extern void fputc(FILE* file, char ch);
extern void fflush(FILE* file);
extern void fclear(FILE* file);
