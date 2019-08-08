#include <dev/video.h>
#include <lib/string.h>

u8 const *video_memory = VIDEO_MEMORY_ADDR;
u8 const video_memory_buff[VIDEO_SCREEN_SIZE * 2];

/*
 * Api - Init video
 */
extern void video_init()
{
  memset(video_memory_buff, VIDEO_MEMORY_ATTR_BLACK, VIDEO_SCREEN_SIZE);
}

/*
 * Api - Scroll video buffer up
 *   Returns new position
 */
extern void *video_scroll(char *video_buff, char *pos)
{
  char *ptr = video_buff;

  /* scroll up */
  for (int i = 1; i < VIDEO_SCREEN_HEIGHT; ++i)
  {
    for (int j = 0; j < VIDEO_SCREEN_WIDTH; ++j)
    {
      ptr[(i - 1) * VIDEO_SCREEN_WIDTH + j] = ptr[i * VIDEO_SCREEN_WIDTH + j];
    }
  }

  /* empty last line */
  for (int j = 0; j < VIDEO_SCREEN_WIDTH; ++j)
  {
    ptr[(VIDEO_SCREEN_HEIGHT - 1) + j] = ' ';
  }

  /* move position up */
  pos -= VIDEO_SCREEN_WIDTH;
}

/*
 * Api - Clear video buff
 */
extern char *video_clear(char *video_buff)
{
  char *ptr = video_buff;

  for (int i = 0; i < VIDEO_SCREEN_SIZE; ++i)
  {
    *ptr++ = ' '; /* blank character */
  }

  return video_buff;
}

/*
 * Api - Copy video buffer to video memory
 */
extern void video_flush(char *video_buff)
{
  memext(video_memory_buff, VIDEO_SCREEN_SIZE, video_buff, VIDEO_MEMORY_ATTR_BLACK);
  memcpy(video_memory, video_memory_buff, VIDEO_SCREEN_SIZE * 2);
}
