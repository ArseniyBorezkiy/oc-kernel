#include <dev/video.h>
#include <utils/kprint.h>
#include <lib/string.h>
#include <messages.h>

static char const syslog[SYSLOG_SIZE]; /* system log */
char *syslog_pos = syslog;             /* system log position */
bool is_early_mode = true;             /* whether syslog attached to video memory */

static void kflush();       /* copy sylog to video memory */
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
extern void kprint(const char *format, ...)
{
    va_list list;
    va_start(list, format);

    kvprint(format, list);
}

/*
 * Api - Print kernel message
 */
extern void kvprint(const char *format, va_list list)
{
    char buff[VIDEO_SCREEN_WIDTH];
    int len = vsprintf(buff, format, list);
    for (int i = 0; i < len; ++i)
    {
        if (buff[i] != '\n')
        {
            kputc(buff[i]);
        }
        else
        {
            int line_pos = (syslog_pos - syslog) % VIDEO_SCREEN_WIDTH;
            for (int j = 0; j < VIDEO_SCREEN_WIDTH - line_pos; ++j)
            {
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
 * Flush syslog to video memory
 */
static void kflush()
{
    if (is_early_mode)
    {
        video_flush(syslog);
    }
}

/*
 * Put character to syslog
 */
static void kputc(char ch)
{
    if (syslog_pos + 1 < SYSLOG_SIZE)
    {
        *syslog_pos++ = ch;
    }
    else
    {
        syslog_pos = video_scroll(syslog, syslog_pos);
        kputc(ch);
    }
}
