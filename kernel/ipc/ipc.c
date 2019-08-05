#include <sched/sched.h>
#include <sched/task.h>
#include <ipc/ipc.h>
#include <lib/string.h>
#include <lib/stdtypes.h>
#include <messages.h>

/*
 * Api - Send message to task
 */
extern void ksend(u_short tid, struct message_t *msg) {
    struct sched_task *task;
    u_short index = sched_find_task_index(tid);

    /* check task found */
    if (index == -1) {
        kpanic(MSG_SCHED_TID_UNKNOWN);
    }

    task = sched_get_task_by_index(index);

    /* check buffer size */
    if (task->msg_count_in == TASK_MSG_BUFF_SIZE) {
        kpanic("message buffer exceed for tid %X", tid);
    }

    /* append message to buffer */
    memcpy(&task->msg_buff[task->msg_count_in++], msg, sizeof(struct message_t));

    /* change task status */
    if (task->status == TASK_INTERRUPTABLE) {
        task->status = TASK_RUNNING;
    }
}

/*
 * Api - Receive message to task
 *   This function has blocking behaviour
 */
extern void kreceive(u_short tid, struct message_t *msg) {
    sched_set_task_status_by_id(tid, TASK_INTERRUPTABLE);
    sched_yield();
}
