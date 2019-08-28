#include <arch/ih.h>
#include <dev/utils/video.h>
#include <ipc/ipc.h>
#include <sched/task.h>
#include <dev/tty.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <lib/syscall.h>

extern FILE *stdin = null;
extern FILE *stdout = null;

/*
 * Api - get stdin
 */

/*
 * Api - Print string to screen
 */
extern void puts(const char *str)
{
    fputs(stdout, str);
}

/*
 * Api - Print character to screen
 */
extern void putc(char ch)
{
    fputc(stdout, ch);
}

/*
 * Api - Clear screen
 */
extern void uclear()
{
    fuclear(stdout);
}

/*
 * Api - Flush screen
 */
extern void flush()
{
    fflush(stdout);
}

/*
 * Api - Print to screen
 */
extern void uprintf(char *format, ...)
{
    va_list list;
    va_start(list, format);

    uvprintf(format, list);
}

/*
 * Api - Print to screen
 */
extern void unprintf(char *format, u_int n, ...)
{
    va_list list;
    va_start(list, n);

    uvnprintf(format, n, list);
}

/*
 * Api - Print user message
 */
extern void uvprintf(const char *format, va_list list)
{
    char buff[VIDEO_SCREEN_WIDTH];
    vsprintf(buff, format, list);
    puts(buff);
}

/*
 * Api - Print user message
 */
extern void uvnprintf(const char *format, u_int n, va_list list)
{
    char buff[VIDEO_SCREEN_WIDTH];
    vsnprintf(buff, n, format, list);
    puts(buff);
}

/*
 * Api - Print string
 */
extern void fputs(FILE *file, const char *str)
{
    asm_syscall(SYSCALL_WRITE, file, str, strlen(str));
}

/*
 * Api - Print character
 */
extern void fputc(FILE *file, char ch)
{
    asm_syscall(SYSCALL_WRITE, file, &ch, 1);
}

/*
 * Api - Flush
 */
extern void fflush(FILE *file)
{
    asm_syscall(SYSCALL_IOCTL, file, IOCTL_FLUSH);
}

/*
 * Api - Clear
 */
extern void fuclear(FILE *file)
{
    asm_syscall(SYSCALL_IOCTL, file, IOCTL_CLEAR);
}
