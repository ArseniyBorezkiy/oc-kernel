#pragma once

#include <lib/stdtypes.h>
#include <arch/reg.h>
#include <ipc/ipc.h>

#define TASK_MAX_COUNT 16
#define TASK_STACK_SIZE (4 * 1024)
#define TASK_MSG_BUFF_SIZE 8

#define TID_INIT 1
#define TID_TTY 2

#define TASK_UNINTERRUPTABLE 1
#define TASK_INTERRUPTABLE 2
#define TASK_RUNNING 3

/*
 * Process descriptor
 */
struct sched_task {
    u_short tid; /* task id */
    bool is_valid; /* whether entry is valid task */
    struct gp_registers_t gp_registers; /* general purpose registers */
    struct op_registers_t op_registers; /* other purpose registers */
    struct flags_t flags; /* processor flags */
    u_int time; /* time of task execution */
    bool reschedule; /* whether task need to be rescheduled */
    u_short status; /* task status */
    int msg_count_in; /* count of incomming messages */
    struct message_t msg_buff[TASK_MSG_BUFF_SIZE]; /* task message buffer */
};

/*
 * Api
 */
extern void task_init();
extern bool task_create(u_short tid, void *address);
extern struct sched_task *task_get_by_id(u_short tid);
extern struct sched_task *task_get_by_index(int index);
extern bool task_set_status_by_id(u_short tid, u_short status);
extern int task_find_to_run_index(int index);
extern int task_find_index(u_short tid);
extern void task_pack_message(u_short tid, struct message_t *msg);
extern void task_extract_message(u_short tid, struct message_t *msg);
