#include <tasks/sh.h>
#include <tasks/tty.h>
#include <sched/task.h>
#include <ipc/ipc.h>
#include <utils/kassert.h>
#include <lib/string.h>
#include <lib/stdio.h>

static char command[SH_CMD_BUFF_SIZE];
static char *cmd_ptr;

static void handle_getc(char ch);
static void execute_command(char *cmd);
static void print_prompt();
static void show_tasks_list();
static void show_system_log();

static const char *cmd_ps = "ps";
static const char *cmd_syslog = "syslog";
static const char *prompt = "# ";

/*
 * Api - Shell task
 */
extern void task_sh_main()
{
  while(1){}
    struct message_t msg;

    /* occupy stdin */
    msg.type = TTY_MSG_TYPE_STDIN;
    msg.len = 2;
    *(u16 *)(&msg.data) = TID_SH;
    ksend(TID_TTY, &msg);

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
            kassert(__FILE__, __LINE__, msg.len == 1);
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

/*
 * Show tasks list
 */
static void show_tasks_list()
{
    clear();
    puts(" -- tasks list\n");

    // snprintf(buf, IPC_MSG_DATA_BUFF_SIZE, "  tid = %u  status = %u  esp = %X\n", task->tid, task->status, task->gp_registers.esp);
    // puts(buf);
}

/*
 * Show system log
 */
static void show_system_log()
{
    clear();
    puts(" -- system log\n");
}
