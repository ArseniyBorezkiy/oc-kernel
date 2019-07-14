#include <utils/kprint.h>
#include <arch/memory.h>

char* video_ptr = (char*)VIDEO_MEMORY_ADDR;
unsigned int video_ptr_offset = 0;
unsigned int screensize = 80 * 25 * 2;

/*
 * Clear kernel screen
 */
void kclear() {
    /*
     * this loops clears the screen
     * there are 25 lines each of 80 columns
     * each element takes 2 bytes
     */
    while (video_ptr_offset < screensize) {
        video_ptr[video_ptr_offset] = ' '; /* blank character */
        video_ptr[video_ptr_offset + 1] = 0x07; /* attribute-byte */
        video_ptr_offset += 2;
    }
}

/*
 * Print kernel message
 */
void kprint(const char *str) {
    unsigned int i = 0;
    unsigned int j = 0;
    while (str[j] != '\0') {
        video_ptr[i] = str[j]; /* the character's ascii */
        video_ptr[i+1] = 0x07; /* attribute-byte */
        ++j;
        i = i + 2;
    }
}
