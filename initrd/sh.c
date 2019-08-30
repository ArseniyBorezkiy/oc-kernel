#include <lib/stdio.h>
#include <lib/string.h>
#include <lib/sys.h>
#include <lib/time.h>
#include <messages.h>

int main();
static bool execute_command(char* cmd);

/*
 * Data
 */
static const char* cmd_ps = "ps";
static const char* cmd_syslog = "syslog";
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
        //show_tasks_list();
    } else if (!strcmp(cmd, cmd_syslog)) {
        /* show system log */
        //show_system_log();
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
        kill(tid);
    } else if (!strncmp(cmd, cmd_exit, strlen(cmd_exit))) {
        /* exit */
        return false;
    }

    return true;
}
