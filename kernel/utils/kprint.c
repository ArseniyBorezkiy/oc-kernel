#include <utils/kprint.h>
#include <arch/memory.h>
#include <lib/math.h>
#include <lib/stdarg.h>
#include <lib/string.h>
#include <types.h>

char* video_ptr = (char*)VIDEO_MEMORY_ADDR;
size_t video_ptr_offset = 0;

/*
 * Clear kernel screen
 */
void kclear() {
    /*
     * this loops clears the screen
     * there are 25 lines each of 80 columns
     * each element takes 2 bytes
     */
    size_t i = 0;
    while (i < SCREEN_SIZE) {
        video_ptr[i++] = ' '; /* blank character */
        video_ptr[i++] = VIDEO_MEMORY_ATTR; /* attribute-byte */
    }
    video_ptr_offset = 0;
}

/*
 * Print kernel message
 */
void kprint(const char *str, ...) {
    size_t j = 0;
    u_int number;
    char str_num[16];

    va_list list;
    va_start(list, str);

    while (str[j] != '\0' && video_ptr_offset < SCREEN_SIZE) {
        if (str[j] != '\n' && str[j] != '%') {
            video_ptr[video_ptr_offset++] = str[j++]; /* the character's ascii */
            video_ptr[video_ptr_offset++] = VIDEO_MEMORY_ATTR; /* attribute-byte */
        } else if (str[j] == '%') {
            switch (str[++j]) {
                case 'c':
                    video_ptr[video_ptr_offset++] = va_arg(list, char);
                    video_ptr[video_ptr_offset++] = VIDEO_MEMORY_ATTR; /* attribute-byte */
                    break;
                case 'u':
                    number = va_arg(list, u_int);
                    itoa(number, str_num, 10);
                    strext(&video_ptr[video_ptr_offset], str_num, VIDEO_MEMORY_ATTR);
                    video_ptr_offset += strlen(str_num) * 2;
                    break;
                case 'X':
                    number = va_arg(list, u_int);
                    itoa(number, str_num, 16);
                    strext(&video_ptr[video_ptr_offset], str_num, VIDEO_MEMORY_ATTR);
                    video_ptr_offset += strlen(str_num) * 2;
                    break;
            }
            j += 1;
        } else {
            size_t offset = video_ptr_offset % SCREEN_WIDTH;
            video_ptr_offset += (SCREEN_WIDTH - offset);
            video_ptr_offset = min(SCREEN_SIZE, video_ptr_offset);
            j++;
        }
    }

    va_end(list);
}
