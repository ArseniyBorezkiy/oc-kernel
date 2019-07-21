#include <utils/kprint.h>
#include <arch/memory.h>
#include <lib/math.h>
#include <lib/stdarg.h>
#include <lib/string.h>

char* video_ptr = (char*)VIDEO_MEMORY_ADDR;
unsigned int video_ptr_offset = 0;

/*
 * Clear kernel screen
 */
void kclear() {
    /*
     * this loops clears the screen
     * there are 25 lines each of 80 columns
     * each element takes 2 bytes
     */
    unsigned int i = 0;
    while (i < SCREEN_SIZE) {
        video_ptr[i++] = ' '; /* blank character */
        video_ptr[i++] = 0x07; /* attribute-byte */
    }
    video_ptr_offset = 0;
}

/*
 * Print kernel message
 */
void kprint(const char *str, ...) {
    unsigned int j = 0;
    unsigned int number;
    unsigned char digit;
    char buffer[16];
    char *current;

    va_list list;
    va_start(list, str);

    while (str[j] != '\0' && video_ptr_offset < SCREEN_SIZE) {
        if (str[j] != '\n' && str[j] != '%') {
            video_ptr[video_ptr_offset++] = str[j++]; /* the character's ascii */
            video_ptr[video_ptr_offset++] = 0x07; /* attribute-byte */
        } else if (str[j] == '%') {
            switch (str[++j]) {
                case 'c':
                    video_ptr[video_ptr_offset++] = va_arg(list, char);
                    video_ptr[video_ptr_offset++] = 0x07; /* attribute-byte */
                    break;
                case 'u':
                    number = va_arg(list, unsigned int);
                    current = &buffer[0];
                    do {
                        digit = number % 10;
                        number = number / 10;
                        *current++ = digit | 0x30;
                        *current++ = 0x07; /* attribute-byte */
                    } while (number > 0);
                    *current = 0;
                    strcpy(&video_ptr[video_ptr_offset], &buffer[0]);
                    video_ptr_offset += strlen(&buffer[0]);
                    break;
                case 'X':
                    video_ptr[video_ptr_offset++] = '0';
                    video_ptr[video_ptr_offset++] = 0x07; /* attribute-byte */
                    video_ptr[video_ptr_offset++] = 'x';
                    video_ptr[video_ptr_offset++] = 0x07; /* attribute-byte */
                    number = va_arg(list, unsigned int);
                    current = &buffer[0];
                    do {
                        digit = number % 16;
                        number = number / 16;
                        if (digit < 10) {
                            *current++ = digit | 0x30;
                        } else {
                            *current++ = (16 - digit) | 0x41;
                        }
                        *current++ = 0x07; /* attribute-byte */
                    } while (number > 0);
                    *current = 0;
                    strcpy(&video_ptr[video_ptr_offset], &buffer[0]);
                    video_ptr_offset += strlen(&buffer[0]);
                    break;
            }
            j += 1;
        } else {
            unsigned int offset = video_ptr_offset % SCREEN_WIDTH;
            video_ptr_offset += (SCREEN_WIDTH - offset);
            video_ptr_offset = min(SCREEN_SIZE, video_ptr_offset);
            j++;
        }
    }

    va_end(list);
}
