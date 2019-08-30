#pragma once

#include <lib/stdtypes.h>

/* screen params */
#define VIDEO_SCREEN_WIDTH 80
#define VIDEO_SCREEN_HEIGHT 25
#define VIDEO_SCREEN_SIZE (VIDEO_SCREEN_WIDTH * VIDEO_SCREEN_HEIGHT)

/* video memory */
#define VIDEO_MEMORY_ADDR 0xb8000
#define VIDEO_MEMORY_ATTR_BLACK 0x07

/*
 * Api
 */
extern void video_init();
extern void video_disable_cursor();
extern void* video_scroll(char const* video_buff, char* pos);
extern char* video_clear(char const* video_buff);
extern void video_flush(char const* video_buff);
