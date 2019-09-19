#include <lib/stdio.h>
#include <lib/string.h>
#include <lib/sys.h>
#include <lib/time.h>
#include <sched/sched.h>
#include <dev/dev.h>
#include <messages.h>

int main();
static bool execute_command(char* cmd);
static void print_task_info(struct clist_head_t* current, va_list list);
static void print_dev_info(struct clist_head_t* current, va_list list);
static void print_dev_ih_info(struct clist_head_t* current, va_list list);

/*
 * Data
 */
static const char* cmd_ps = "ps";
static const char* cmd_clear = "clear";
static const char* cmd_kill = "kill";
static const char* cmd_exit = "exit";
static const char* cmd_exec = "exec";
static const char* cmd_dev = "dev";
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
        clear();
        flush();
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
        clear();
        printf(prompt);
        flush();
        return false;
    } else if (!strncmp(cmd, cmd_exec, strlen(cmd_exec))) {
        /* exec file on intrd */
        char* save_ptr = null;
        strtok_r(cmd, " ", &save_ptr);
        char* str_file = strtok_r(null, " ", &save_ptr);
        exec(str_file);
    } else if (!strncmp(cmd, cmd_dev, strlen(cmd_dev))) {
        /* show device list */
        struct clist_definition_t *dev_list;
        dev_list = devs();
        printf(" -- device list\n");
        clist_for_each(dev_list, print_dev_info);
    } else {
        printf("  There is no such command '%s'.\n  Available command list:\n", cmd);
        printf("    %s  %s  %s <pid>  %s <file.elf>  %s  %s\n", cmd_ps, cmd_exit, cmd_kill, cmd_exec, cmd_clear, cmd_dev);
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

/*
 * Print device info
 */
static void print_dev_info(struct clist_head_t* current, va_list list) {
    struct dev_t *dev;
    dev = (struct dev_t *)current->data;
    printf("  + %s\n", dev->name);
    clist_for_each(&dev->ih_list, print_dev_ih_info);
}

/*
 * Print device interrupt handler info
 */
static void print_dev_ih_info(struct clist_head_t* current, va_list list) {
    struct ih_low_t *ih;
    ih = (struct ih_low_t *)current->data;
    printf("    handles interrupt %X (handler %X) \n", ih->number, ih->handler);
}
