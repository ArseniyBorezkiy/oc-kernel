#pragma once

#include <lib/stdtypes.h>
#include <arch/registers.h>
#include <ipc/ipc.h>

#define MAX_TASKS_COUNT 16
#define TASK_STACK_SIZE (4 * 1024)
#define TASK_MSG_BUFF_SIZE 8

#define TID_INIT 1
#define TID_TTY 2

#define TASK_STATUS_UNINTERRUPTABLE 1
#define TASK_STATUS_RUNNING 2
#define TASK_STATUS_PENDING 3

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
    u_short status; /* task status */
    u_short msg_count_in; /* count of incomming messages */
    struct message_t msg_buff[TASK_MSG_BUFF_SIZE]; /* task message buffer */
};

/*
 * Api
 */
extern void sched_init();
extern bool sched_create_task(u_short tid, void *address);
extern struct sched_task *sched_get_task_by_index(int index);
extern bool sched_run_task_by_id(u_short tid);
extern bool sched_stop_task_by_id(u_short tid);
extern bool sched_set_task_status_by_id(u_short tid, u_short status);
extern int sched_find_task_to_run_index(int index);
extern int sched_find_task_index(u_short tid);
