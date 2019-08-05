#include <sched/sched.h>
#include <sched/task.h>
#include <ipc/ipc.h>
#include <utils/kassert.h>
#include <lib/string.h>
#include <lib/stdtypes.h>
#include <messages.h>

/*
 * Api - Send message to task
 */
extern void ksend(u_short tid, struct message_t *msg) {
    struct sched_task *task;

    /* get target task */
    task = task_get_by_id(tid);
    /* put message to task buffer */
    task_pack_message(tid, msg);
    /* whether task has frozen */
    if (task->status == TASK_INTERRUPTABLE) {
        /* defrost task */
        task->status = TASK_RUNNING;
    }
}

/*
 * Api - Receive message to task
 *   This function has blocking behaviour
 */
extern void kreceive(u_short tid, struct message_t *msg) {
    struct sched_task *task_before; /* before yield */
    struct sched_task *task_after; /* after yield */

    /* get current task */
    task_before = sched_get_current_task();
    kassert(__FILE__, __LINE__, tid == task_before->tid);
    kassert(__FILE__, __LINE__, task_before->status == TASK_RUNNING);
    /* whether task has not incomming messages */
    if (task_before->msg_count_in == 0) {
        /* freeze task */
        task_set_status_by_id(tid, TASK_INTERRUPTABLE);
    }
    /* wait fot message */
    sched_yield();
    /* get current task */
    task_after = sched_get_current_task();
    kassert(__FILE__, __LINE__, task_after == task_before);
    kassert(__FILE__, __LINE__, tid == task_after->tid);
    kassert(__FILE__, __LINE__, task_after->status == TASK_RUNNING);
    /* fetch message from buffer */
    task_extract_message(tid, msg);
    kassert(__FILE__, __LINE__, msg != null);
}
