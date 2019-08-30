#include <lib/stdio.h>
#include <lib/time.h>
#include <messages.h>

static void message_loop();

/*
 * Elf entry point
 */
extern int main()
{
    stdio_init();
    printf(MSG_KERNEL_NAME);
    message_loop();
    stdio_deinit();

    return 0;
}

/*
 * Main message loop
 */
static void message_loop()
{
    char buff[255];

    while (1) {
        fread(stdin, buff, 255);
        printf("%s", buff);
        delay(1000);
    }
}
