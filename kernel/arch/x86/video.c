#include <arch/arch.h>
#include <dev/utils/video.h>
#include <lib/string.h>

u8 const* video_memory = (void*)VIDEO_MEMORY_ADDR;
u8 const video_memory_buff[VIDEO_SCREEN_SIZE * 2];

/*
 * Api - Init video
 */
extern void video_init()
{
    memset((void*)video_memory_buff, VIDEO_MEMORY_ATTR_BLACK, VIDEO_SCREEN_SIZE);
    video_disable_cursor();
}

/*
 * Api - Disable cursor
 */
extern void video_disable_cursor()
{
    asm_write_port(0x3D4, 0x0A);
    asm_write_port(0x3D5, 0x20);
}

/*
 * Api - Scroll video buffer up
 *   Returns new position
 */
extern void* video_scroll(char const* video_buff, char* pos)
{
    char* ptr = (void*)video_buff;

    /* scroll up */
    for (int i = 1; i < VIDEO_SCREEN_HEIGHT; ++i) {
        for (int j = 0; j < VIDEO_SCREEN_WIDTH; ++j) {
            ptr[(i - 1) * VIDEO_SCREEN_WIDTH + j] = ptr[i * VIDEO_SCREEN_WIDTH + j];
        }
    }

    /* empty last line */
    for (int j = 0; j < VIDEO_SCREEN_WIDTH; ++j) {
        ptr[(VIDEO_SCREEN_HEIGHT - 1) * VIDEO_SCREEN_WIDTH + j] = ' ';
    }

    /* move position up */
    pos -= VIDEO_SCREEN_WIDTH;

    return pos;
}

/*
 * Api - Clear video buff
 */
extern char* video_clear(char const* video_buff)
{
    char* ptr = (void*)video_buff;

    for (int i = 0; i < VIDEO_SCREEN_SIZE; ++i) {
        *ptr++ = ' '; /* blank character */
    }

    return (void*)video_buff;
}

/*
 * Api - Copy video buffer to video memory
 */
extern void video_flush(char const* video_buff)
{
    memext((void*)video_memory_buff, VIDEO_SCREEN_SIZE, video_buff, VIDEO_MEMORY_ATTR_BLACK);
    memcpy((void*)video_memory, video_memory_buff, VIDEO_SCREEN_SIZE * 2);
}
