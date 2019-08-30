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
    printf("Unix like shell\n");
    flush();
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
        scanf(buff);
        delay(1);
    }
}
