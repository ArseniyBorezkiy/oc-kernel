#include <arch/arch.h>
#include <dev/utils/video.h>
#include <lib/assert.h>
#include <lib/string.h>
#include <messages.h>
#include <utils/kprint.h>

static char const syslog[SYSLOG_SIZE]; /* system log */
char* syslog_pos = (char*)syslog; /* system log position */
bool is_early_mode = true; /* whether syslog attached to video memory */

static void kflush(); /* copy sylog to video memory */
static void kputc(char ch); /* put character to syslog */

/*
 * Api - Clear kernel screen
 */
extern void kclear()
{
    syslog_pos = video_clear(syslog);
    kflush();
}

/*
 * Api - Print kernel message
 */
extern void kprintf(const char* format, ...)
{
    va_list list;
    va_start(list, format);

    kvprintf(format, list);
}

/*
 * Api - Print kernel message
 */
extern void kvprintf(const char* format, va_list list)
{
    char buff[VIDEO_SCREEN_WIDTH];
    int len = vsprintf(buff, format, list);

    for (int i = 0; i < len; ++i) {
        if (buff[i] != '\n') {
            kputc(buff[i]);
        } else {
            int line_pos = (syslog_pos - syslog) % VIDEO_SCREEN_WIDTH;
            for (int j = 0; j < VIDEO_SCREEN_WIDTH - line_pos; ++j) {
                kputc(' ');
            }
        }
    }

    kflush();
}

/*
 * Api - Set print mode
 */
extern void kmode(bool is_early)
{
    is_early_mode = is_early;
}

/*
 * Api - Read syslog to buffer
 */
extern void klog(char* buf, u_int n)
{
    assert(n > 0);
    assert(n < VIDEO_MEMORY_ADDR);
    asm_lock();

    u_int syslog_end_line = (syslog_pos - syslog) / VIDEO_SCREEN_WIDTH;
    u_int syslog_start_line = syslog_end_line - n;
    const void* syslog_begin = syslog + syslog_start_line * VIDEO_SCREEN_WIDTH;
    memcpy(buf, syslog_begin, VIDEO_SCREEN_WIDTH * n);

    asm_unlock();
}

/*
 * Flush syslog to video memory
 */
static void kflush()
{
    if (is_early_mode) {
        video_flush(syslog);
    }
}

/*
 * Put character to syslog
 */
static void kputc(char ch)
{
    if ((size_t)syslog_pos - (size_t)syslog + 1 < SYSLOG_SIZE) {
        *syslog_pos++ = ch;
    } else {
        syslog_pos = video_scroll(syslog, syslog_pos);
        kputc(ch);
    }
}
