#include <tasks/init.h>
#include <utils/kprint.h>
#include <messages.h>

/*
 * Api - Init task
 */
extern void task_init() {
    while (1) {
        kprint(MSG_TASK_INIT_LAUNCHED);
    }
}
