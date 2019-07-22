#include <sched/task.h>
#include <sched/sched.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <messages.h>

int current_task_index = -1;

/*
 * Api - Schedule task to run
 */
extern void sched_schedule(size_t *task_ret_addr) {
    current_task_index = sched_find_task_to_run_index(current_task_index);

    /* check task found */
    if (current_task_index == -1) {
        kpanic(MSG_SCHED_NO_TASKS);
    }
    
    kprint("SCHED %u", current_task_index);
}
