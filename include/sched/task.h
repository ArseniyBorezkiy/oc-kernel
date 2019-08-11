#pragma once

#include <lib/stdtypes.h>
#include <arch/reg.h>
#include <ipc/ipc.h>
#include <lib/data/clist.h>

/* limits */
#define TASK_MAX_COUNT 16
#define TASK_KSTACK_SIZE 1024
#define TASK_USTACK_SIZE (4 * 1024)
#define TASK_MSG_BUFF_SIZE 16

/* predefined tasks */
#define TID_INIT 1
#define TID_TTY 2
#define TID_SH 3

/* task status */
#define TASK_UNINTERRUPTABLE 1
#define TASK_INTERRUPTABLE 2
#define TASK_RUNNING 3

/*
 * Process descriptor
 */
struct task_t
{
    struct clist_head_t list_head;                 /* should be at first */
    u_short tid;                                   /* task id */
    struct gp_registers_t gp_registers;            /* general purpose registers */
    struct op_registers_t op_registers;            /* other purpose registers */
    struct flags_t flags;                          /* processor flags */
    u_int time;                                    /* time of task execution */
    bool reschedule;                               /* whether task need to be rescheduled */
    u_short status;                                /* task status */
    int msg_count_in;                              /* count of incomming messages */
    struct message_t msg_buff[TASK_MSG_BUFF_SIZE]; /* task message buffer */
    void *kstack;                                  /* kernel stack top */
    void *ustack;                                  /* user stack top */
} attribute(packed);

typedef void (*task_each_callback_t)(struct task_t *entry);

/*
 * Api
 */
extern void task_init();
extern bool task_create(u_short tid, void *address);
extern void task_delete(struct task_t *task);
extern struct task_t *task_get_by_id(u_short tid);
extern struct task_t *task_find_by_id(u_short tid);
extern struct task_t *task_get_by_status(u_short status);
extern struct task_t *task_get_next_by_status(u_short status, struct task_t *pos);
extern void task_pack_message(struct task_t *task, struct message_t *msg);
extern void task_extract_message(struct task_t *task, struct message_t *msg);
extern void task_dump();
extern void task_for_each(task_each_callback_t callback);
