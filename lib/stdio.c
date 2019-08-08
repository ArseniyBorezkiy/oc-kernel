#include <ipc/ipc.h>
#include <sched/task.h>
#include <tasks/tty.h>
#include <lib/string.h>
#include <lib/stdio.h>

/*
 * Api - Print string to screen
 */
extern void puts(const char *str)
{
    struct message_t msg;

    msg.type = TTY_MSG_TYPE_PUTS;
    msg.len = strlen(str) + 1;
    strcpy((char *)&msg.data, str);
    ksend(TID_TTY, &msg);
}

/*
 * Api - Print character to screen
 */
extern void putc(char ch)
{
    struct message_t msg;

    msg.type = TTY_MSG_TYPE_PUTC;
    msg.len = 1;
    msg.data[0] = ch;
    ksend(TID_TTY, &msg);
}

/*
 * Api - Clear screen
 */
extern void clear()
{
    struct message_t msg;

    msg.type = TTY_MSG_TYPE_CLEAR;
    msg.len = 0;
    ksend(TID_TTY, &msg);
}
