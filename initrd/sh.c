#include <lib/stdio.h>
#include <lib/string.h>
#include <lib/sys.h>
#include <lib/time.h>
#include <sched/sched.h>
#include <messages.h>

int main();
static bool execute_command(char* cmd);
static void print_task_info(struct clist_head_t* current, va_list list);

/*
 * Data
 */
static const char* cmd_ps = "ps";
static const char* cmd_clear = "clear";
static const char* cmd_kill = "kill";
static const char* cmd_exit = "exit";
static const char* prompt = "# ";

/*
 * Elf entry point
 */
void start()
{
    u_int errno;
    stdio_init();
    errno = main();
    stdio_deinit();
    exit(errno);
}

/*
 * Main message loop
 */
int main()
{
    char cmd[255];

    while (1) {
        printf(prompt);
        flush();
        scanf(cmd);
        if (!execute_command(cmd)) {
            break;
        }
    }

    return 0;
}

/*
 * Execute command
 */
static bool execute_command(char* cmd)
{
    if (!strcmp(cmd, cmd_ps)) {
        /* show tasks list */
        struct clist_definition_t *task_list;
        task_list = ps();
        printf(" -- process list\n");
        clist_for_each(task_list, print_task_info);
    } else if (!strcmp(cmd, cmd_clear)) {
        /* clear screen */
        uclear();
        uflush();
    } else if (!strncmp(cmd, cmd_kill, strlen(cmd_kill))) {
        /* kill task */
        char* save_ptr = null;
        strtok_r(cmd, " ", &save_ptr);
        char* str_tid = strtok_r(null, " ", &save_ptr);
        u_short tid = atou(str_tid);
        if (!kill(tid)) {
            printf("  There is no process with pid %u\n", tid);
        };
    } else if (!strncmp(cmd, cmd_exit, strlen(cmd_exit))) {
        /* exit */
        return false;
    } else {
        printf("  There is no such command.\n  Available command list:\n");
        printf("    %s  %s  %s <pid>  %s\n", cmd_ps, cmd_exit, cmd_kill, cmd_clear);
    }

    return true;
}

/*
 * Print task info
 */
static void print_task_info(struct clist_head_t* current, va_list list) {
    struct task_t *task;
    task = (struct task_t *)current->data;
    printf("  %s, tid = %u, status = %X\n", task->name, task->tid, task->status);
}
