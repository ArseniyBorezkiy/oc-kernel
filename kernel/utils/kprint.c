#include <utils/kprint.h>
#include <arch/memory.h>
#include <lib/math.h>

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
void kprint(const char *str) {
    unsigned int j = 0;
    while (str[j] != '\0' && video_ptr_offset < SCREEN_SIZE) {
        if (str[j] != '\n') {
            video_ptr[video_ptr_offset++] = str[j++]; /* the character's ascii */
            video_ptr[video_ptr_offset++] = 0x07; /* attribute-byte */
        } else {
            unsigned int offset = video_ptr_offset % SCREEN_WIDTH;
            video_ptr_offset += (SCREEN_WIDTH - offset);
            video_ptr_offset = min(SCREEN_SIZE, video_ptr_offset);
            j++;
        }
    }
}
