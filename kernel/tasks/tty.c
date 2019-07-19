#include <tasks/tty.h>
#include <utils/kprint.h>
#include <messages.h>

/*
 * Teletype task
 */
void task_tty() {
    while (1) {
        kprint(MSG_TASK_TTY_LAUNCHED);
    }
}
