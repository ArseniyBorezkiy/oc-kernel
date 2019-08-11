#pragma once

#define TTY_MSG_TYPE_GETC 1
#define TTY_MSG_TYPE_PUTC 2
#define TTY_MSG_TYPE_PUTS 3
#define TTY_MSG_TYPE_CLEAR 4
#define TTY_MSG_TYPE_STDIN 5
#define TTY_MSG_TYPE_STDOUT 6
#define TTY_MSG_TYPE_PRINTF 7

/*
 * Api
 */
extern void task_tty_main();
