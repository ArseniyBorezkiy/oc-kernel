#include <arch/arch.h>
#include <dev/tty.h>
#include <dev/utils/video.h>
#include <ipc/ipc.h>
#include <lib/stdio.h>
#include <lib/string.h>
#include <lib/syscall.h>
#include <sched/task.h>

/*
 * Data
 */
FILE* stdin = null;
FILE* stdout = null;
static const char* tty_dev_name = TTY_DEV_NAME;

/*
 * Api - Lib init
 */
extern void stdio_init()
{
    stdin = fopen(tty_dev_name, MOD_R);
    stdout = fopen(tty_dev_name, MOD_W);
    asm_syscall(SYSCALL_IOCTL, stdout, IOCTL_INIT);
    asm_syscall(SYSCALL_IOCTL, stdin, IOCTL_READ_MODE_LINE);
    asm_syscall(SYSCALL_IOCTL, stdin, IOCTL_READ_MODE_ECHO);
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
extern void uputs(const char* str)
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
extern void uprintf(const char* format, ...)
{
    va_list list;
    va_start(list, format);

    uvprintf(format, list);
}

/*
 * Api - Print to screen
 */
extern void unprintf(const char* format, u_int n, ...)
{
    va_list list;
    va_start(list, n);

    uvnprintf(format, n, list);
}

/*
 * Api - Print user message
 */
extern void uvprintf(const char* format, va_list list)
{
    char buff[VIDEO_SCREEN_WIDTH];
    vsprintf(buff, format, list);
    uputs(buff);
}

/*
 * Api - Print user message
 */
extern void uvnprintf(const char* format, u_int n, va_list list)
{
    char buff[VIDEO_SCREEN_WIDTH];
    vsnprintf(buff, n, format, list);
    uputs(buff);
}

/*
 * Api - Read from file to string
 */
extern void uscanf(char* buff, ...)
{
    u_int readed = 0;
    do {
        readed = fread(stdin, buff, 255);
    } while (readed == 0);
    buff[readed] = '\0';
    uprintf("\n");
    uflush();
}

//
// File API
//

/*
 * Api - Open file
 */
extern FILE* fopen(const char* file, int mod_rw)
{
    FILE* result = null;
    asm_syscall(SYSCALL_OPEN, file, mod_rw, &result);
    return result;
}

/*
 * Api - Close file
 */
extern void fclose(FILE* file)
{
    asm_syscall(SYSCALL_CLOSE, file);
}

/*
 * Api - Read from file to buffer
 */
extern u_int fread(FILE* file, char* buff, u_int size)
{
    return asm_syscall(SYSCALL_READ, file, buff, size);
}

/*
 * Api - Write data to file
 */
extern void fwrite(FILE* file, const char* data, u_int size)
{
    asm_syscall(SYSCALL_WRITE, file, data, size);
}

/*
 * Api - Print string
 */
extern void fputs(FILE* file, const char* str)
{
    fwrite(file, str, strlen(str));
}

/*
 * Api - Print character
 */
extern void fputc(FILE* file, char ch)
{
    fwrite(file, &ch, 1);
}

/*
 * Api - Flush
 */
extern void fflush(FILE* file)
{
    asm_syscall(SYSCALL_IOCTL, file, IOCTL_FLUSH);
}

/*
 * Api - Clear
 */
extern void fclear(FILE* file)
{
    asm_syscall(SYSCALL_IOCTL, file, IOCTL_CLEAR);
}
