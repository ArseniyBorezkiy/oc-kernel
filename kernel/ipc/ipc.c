#include <ipc/ipc.h>
#include <sched/task.h>

/*
 * Api - send message to task
 */
extern void ksend(u_short tid, struct message_t *msg) {

}

/*
 * Api - receive message to task
 *   This function has blocking behaviour
 */
extern void kreceive(u_short tid, struct message_t *msg) {
    sched_set_task_status_by_id(tid, TASK_STATUS_PENDING);
}
