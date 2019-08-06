#pragma once

#include <lib/stdtypes.h>
#include <arch/reg.h>
#include <ipc/ipc.h>

/* limits */
#define TASK_MAX_COUNT 16
#define TASK_KSTACK_SIZE 1024
#define TASK_USTACK_SIZE (4 * 1024)
#define TASK_MSG_BUFF_SIZE 8

/* predefined tasks */
#define TID_INIT 1
#define TID_TTY 2

/* task status */
#define TASK_UNINTERRUPTABLE 1
#define TASK_INTERRUPTABLE 2
#define TASK_RUNNING 3

/*
 * Process descriptor
 */
struct sched_task_t {
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
    void *kstack; /* kernel stack top */
    void *ustack; /* user stack top */
    struct sched_task_t *next; /* next entry */
    struct sched_task_t *prev; /* prev entry */
};

/*
 * Api
 */
extern void task_init();
extern bool task_create(u_short tid, void *address);
extern void task_delete(struct sched_task_t *task);
extern struct sched_task_t *task_get_by_id(u_short tid);
extern struct sched_task_t *task_find_by_id(u_short tid);
extern struct sched_task_t *task_get_by_status(u_short status, struct sched_task_t *list_head);
extern void task_pack_message(struct sched_task_t *task, struct message_t *msg);
extern void task_extract_message(struct sched_task_t *task, struct message_t *msg);
