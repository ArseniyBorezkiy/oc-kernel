#include <boot/multiboot.h>
#include <lib/string.h>

char *hello = "Hello world!";
int screen_size = 80 * 25;

/*
 * Api - Kernel entry point
 */
extern void kernel_start(struct multiboot_t* multiboot, void* kstack)
{
    char *video = (char*)0xB8000;
    char buff[screen_size + 1];
    video[screen_size] = '\0';

    memset(buff, ' ', screen_size);
    strext(video, buff, 0x7);
    strext(video, hello, 0x7);
}
