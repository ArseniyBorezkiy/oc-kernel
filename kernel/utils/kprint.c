#include <arch/idt.h>
#include <sync/spin.h>
#include <utils/kprint.h>
#include <lib/math.h>
#include <lib/string.h>
#include <lib/stdtypes.h>
#include <messages.h>

static const char syslog[SCREEN_SIZE]; /* kernel system log */
static const char* syslog_ptr;
static char* kprint_buff; /* output buffer address */
static char* kprint_buff_ptr; /* current buffer position */
static struct spin_t video_spin;

/*
 * Api - Init video module
 */
extern void init_video() {
    memset(&video_spin, 0, sizeof(struct spin_t));
    /* early kprint */
    kprint_buff = (char*)VIDEO_MEMORY_ADDR;
    kprint_buff_ptr = kprint_buff;
}

/*
 * Api - Turn on system log
 */
extern void init_syslog() {
    spin_lock(&video_spin);

    syslog_ptr = &syslog;

    memset(syslog_ptr, 0, SCREEN_SIZE);
    kprint(MSG_KERNEL_SYSLOG_INITIALIZED);
    /* syslog kprint */
    kprint_buff = syslog_ptr;
    kprint_buff_ptr = kprint_buff;
    
    spin_unlock(&video_spin);
}

/*
 * Api - Clear kernel screen
 */
extern void kclear() {
    spin_lock(&video_spin);

    kprint_buff_ptr = kprint_buff;

    for (size_t i = 0; i < SCREEN_SIZE; ++i) {
        *kprint_buff_ptr++ = ' '; /* blank character */
        *kprint_buff_ptr++ = VIDEO_MEMORY_ATTR; /* attribute-byte */
    }

    kprint_buff_ptr = kprint_buff;

    spin_unlock(&video_spin);
}

/*
 * Api - Print kernel message
 */
extern void kprint(const char *str, ...) {
    va_list list;
    va_start(list, str);

    kvprint(str, list);
}

/*
 * Api - Print kernel message
 */
extern void kvprint(const char *str, va_list list) {
    char buf[SCREEN_WIDTH];
    char *str;
    u_int num;
    char ch;

    spin_lock(&video_spin);

    while (*str != '\0') {
        if (*str != '\n' && *str != '%') {
            // usual character
            if (kprint_buff_ptr - kprint_buff + 2 >= SCREEN_SIZE) {
                kscroll(1); /* scroll line up */
            }

            *kprint_buff_ptr++ = *str++; /* the character's ascii */
            *kprint_buff_ptr++ = VIDEO_MEMORY_ATTR; /* attribute-byte */
        } else if (*str == '%') {
            // control character
            switch (*++str) {
                case 'c':
                    if (kprint_buff_ptr - kprint_buff + 2 >= SCREEN_SIZE) {
                        kscroll(1); /* scroll line up */
                    }

                    ch = va_arg(list, char);
                    *kprint_buff_ptr++ = ch;
                    *kprint_buff_ptr++ = VIDEO_MEMORY_ATTR;
                    break;
                case 'u':
                    num = va_arg(list, u_int);
                    itoa(num, buf, 10);

                    if (kprint_buff_ptr - kprint_buff + strlen(buf) * 2 >= SCREEN_SIZE) {
                        kscroll(1); /* scroll line up */
                    }

                    kprint_buff_ptr = strext(kprint_buff_ptr, buf, VIDEO_MEMORY_ATTR);
                    break;
                case 'X':
                    num = va_arg(list, u_int);
                    itoa(num, buf, 16);

                    if (kprint_buff_ptr - kprint_buff + strlen(buf) * 2 >= SCREEN_SIZE) {
                        kscroll(1); /* scroll line up */
                    }

                    kprint_buff_ptr = strext(kprint_buff_ptr, buf, VIDEO_MEMORY_ATTR);
                    break;
                case 's':
                    str = va_arg(list, char*);
                    strcpy(buf, str);

                    if (kprint_buff_ptr - kprint_buff + strlen(buf) * 2 >= SCREEN_SIZE) {
                        kscroll(1); /* scroll line up */
                    }

                    kprint_buff_ptr = strext(kprint_buff_ptr, buf, VIDEO_MEMORY_ATTR);
                    break;
            }
        } else if (*str == '\n') {
            // new line character
            size_t offset = (kprint_buff_ptr - kprint_buff) % SCREEN_WIDTH;

            if (kprint_buff_ptr - kprint_buff + offset >= SCREEN_SIZE) {
                kscroll(1); /* scroll line up */
            }

            kprint_buff_ptr += (SCREEN_WIDTH - offset);
        }
    }

    va_end(list);

    spin_unlock(&video_spin);
}

/*
 * Api - Scroll console up
 */
extern void kscroll(u_int n) {
    spin_lock(&video_spin);

    char *ptr = kprint_buff;
    for (int i = 1; i < SCREEN_HEIGHT * 2; ++i) {
        for (int j = 0; j < SCREEN_WIDTH * 2; ++j) {
            ptr[(i - 1) * SCREEN_WIDTH * 2 + j] = ptr[i * SCREEN_WIDTH * 2 + j];
        }
    }

    for (int j = 0; j < SCREEN_WIDTH; ++j) {
        ptr[(SCREEN_HEIGHT * 2 - 1) + j * 2] = ' ';
        ptr[(SCREEN_HEIGHT * 2 - 1) + j * 2 + 1] = VIDEO_MEMORY_ATTR;
    }

    kprint_buff_ptr -= SCREEN_WIDTH * 2;

    spin_unlock(&video_spin);
}
