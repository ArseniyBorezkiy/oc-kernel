#include <utils/kprint.h>
#include <arch/memory.h>

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
    unsigned int screensize = 80 * 25 * 2;
    char *vidptr = (char*)VIDEO_MEMORY_ADDR;
    while (i < screensize) {
        vidptr[i] = ' ';    /* blank character */
        vidptr[i+1] = 0x07; /* attribute-byte */
        i = i + 2;
    }
}

/*
 * Print kernel message
 */
void kprint(const char *str) {
    unsigned int i = 0;
    unsigned int j = 0;
    char *vidptr = (char*)VIDEO_MEMORY_ADDR;
    while (str[j] != '\0') {
        /* the character's ascii */
        vidptr[i] = str[j];
        /* attribute-byte: give character black bg and light grey fg */
        vidptr[i+1] = 0x07;
        ++j;
        i = i + 2;
    }
}
