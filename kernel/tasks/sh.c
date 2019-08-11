#include <tasks/sh.h>
#include <tasks/tty.h>
#include <sched/task.h>
#include <ipc/ipc.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <lib/assert.h>

static char command[SH_CMD_BUFF_SIZE];
static char *cmd_ptr;

static void handle_getc(char ch);
static void execute_command(char *cmd);
static void print_prompt();
static void show_tasks_list();
static void show_system_log();
static void show_task(struct task_t *entry);

static const char *cmd_ps = "ps";
static const char *cmd_syslog = "syslog";
static const char *prompt = "# ";

/*
 * Api - Shell task
 */
extern void task_sh_main()
{
    struct message_t msg;

    /* occupy stdin */
    msg.type = TTY_MSG_TYPE_STDIN;
    msg.len = 2;
    *(u16 *)(&msg.data) = TID_SH;
    ksend(TID_TTY, &msg);
    print_prompt();

    /* init */
    cmd_ptr = command;

    while (1)
    {
        kreceive(TID_SH, &msg);

        /* decode message */
        switch (msg.type)
        {
        case TTY_MSG_TYPE_GETC:
        {
            /* incomming character */
            assert(msg.len == 1);
            handle_getc(msg.data[0]);
            break;
        }
        }
    }
}

/*
 * Handle incomming character
 */
static void handle_getc(char ch)
{
    putc(ch);

    if (ch == SH_CMD_END_CH)
    {
        /* finish command */
        *cmd_ptr++ = 0; /* terminator */
        cmd_ptr = command;
        execute_command(cmd_ptr);
        print_prompt();
    }
    else
    {
        /* append character to command */
        if (cmd_ptr - command < SH_CMD_BUFF_SIZE - 1)
        {
            *cmd_ptr++ = ch;
        }
    }
}

/*
 * Execute command
 */
static void execute_command(char *cmd)
{
    if (!strcmp(cmd, cmd_ps))
    {
        /* show tasks list */
        show_tasks_list();
    }
    else if (!strcmp(cmd, cmd_syslog))
    {
        /* show system log */
        show_system_log();
    }
}

/*
 * Prompt
 */
static void print_prompt()
{
    puts(prompt);
}

//
// Commands
//

/*
 * Show tasks list
 */
static void show_tasks_list()
{
    puts(" -- process list\n");
    task_for_each(show_task);
}

/*
 * Show system log
 */
static void show_system_log()
{
    puts(" -- system log\n");
    for (int i = 0; i < 5; ++i) {
      //char *d = "asdfdsf\n";
      //uprintf(" a%c%s\n", 'd', d);
    }
}

//
// Helpers
//

static void show_task(struct task_t *entry) {
    uprintf("  pid = %u\n", entry->tid);
}
