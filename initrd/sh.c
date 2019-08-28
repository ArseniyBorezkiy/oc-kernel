#include <lib/stdio.h>
#include <messages.h>

static void message_loop();

/*
 * Elf entry point
 */
extern int main()
{
  stdio_init();
  printf(MSG_KERNEL_NAME);
  main_loop();
  stdio_deinit();

  return 0;
}

/*
 * Main message loop
 */
static void message_loop()
{
  while (1)
  {
  }
}
