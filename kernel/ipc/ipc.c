#include <ipc/ipc.h>
#include <lib/assert.h>
#include <lib/stdtypes.h>
#include <lib/string.h>
#include <messages.h>
#include <sched/sched.h>
#include <sched/task.h>

/*
 * Api - Send message to task
 */
extern void ksend(u_short tid, struct message_t* msg)
{
    struct task_t* task;

    /* get target task */
    task = task_get_by_id(tid);
    /* put message to task buffer */
    task_pack_message(task, msg);
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
extern void kreceive(u_short tid, struct message_t* msg)
{
    struct task_t* task_before; /* before yield */
    struct task_t* task_after; /* after yield */

    /* get current task */
    task_before = sched_get_current_task();
    assert(tid == task_before->tid);
    assert(task_before->status == TASK_RUNNING);
    /* whether task has not incomming messages */
    if (task_before->msg_count_in == 0) {
        /* freeze task */
        task_before->status = TASK_INTERRUPTABLE;
    }
    /* wait fot message */
    sched_yield();
    /* get current task */
    task_after = sched_get_current_task();
    assert(task_after == task_before);
    assert(tid == task_after->tid);
    assert(task_after->status == TASK_RUNNING);
    /* fetch message from buffer */
    task_extract_message(task_after, msg);
    assert(msg != null);
}
