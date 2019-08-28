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
 * Api - Lib init
 */
extern void stdio_init()
{
    stdin = fopen(tty_dev_name, MOD_R);
    stdout = fopen(tty_dev_name, MOD_W);
}

/*
 * Api - Lib deinit
 */
extern void stdio_deinit()
{
    fclose(stdin);
    fclose(stdout);
}

//
// Stdio API
//

/*
 * Api - Print string to screen
 */
extern void uputs(const char *str)
{
    fputs(stdout, str);
}

/*
 * Api - Print character to screen
 */
extern void uputc(char ch)
{
    fputc(stdout, ch);
}

/*
 * Api - Clear screen
 */
extern void uclear()
{
    fclear(stdout);
}

/*
 * Api - Flush screen
 */
extern void uflush()
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

//
// File API
//

/*
 * Api - Open file
 */
extern FILE *fopen(char *file, int mod_rw)
{
    FILE *result = null;
    asm_syscall(SYSCALL_OPEN, file, mod_rw, &result);
    return result;
}

/*
 * Api - Close file
 */
extern void fclose(FILE *file)
{
    asm_syscall(SYSCALL_CLOSE, file);
}

/*
 * Api - Read from file to buffer
 */
extern void fread(FILE *file, char *buff, u_int size)
{
    asm_syscall(SYSCALL_READ, file, buff, size);
}

/*
 * Api - Write data to file
 */
extern void fwrite(FILE *file, char *data, u_int size)
{
    asm_syscall(SYSCALL_WRITE, file, data, size);
}

/*
 * Api - Print string
 */
extern void fputs(FILE *file, const char *str)
{
    fwrite(file, str, strlen(str));
}

/*
 * Api - Print character
 */
extern void fputc(FILE *file, char ch)
{
    fwrite(file, &ch, 1);
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
extern void fclear(FILE *file)
{
    asm_syscall(SYSCALL_IOCTL, file, IOCTL_CLEAR);
}
