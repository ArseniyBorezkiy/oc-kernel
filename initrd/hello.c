#include <lib/stdio.h>

/*
 * Elf entry point
 */
extern int main() {
  printf("Hello world from initial ram disk hello.elf!\n");

  while(1) {
    // receive messages
  }

  return 0;
}
