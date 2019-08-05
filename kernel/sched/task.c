#include <arch/idt.h>
#include <arch/reg.h>
#include <sched/task.h>
#include <ipc/ipc.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <lib/string.h>
#include <lib/stdtypes.h>
#include <assembly.h>
#include <messages.h>

static int task_get_free_index();
static void task_test();

/*
 * Tasks
 */

static struct sched_task tasks[TASK_MAX_COUNT];
static void *stacks[TASK_MAX_COUNT][TASK_STACK_SIZE];

/*
 * Api - Init
 */
extern void task_init() {
    memset(tasks, 0, sizeof(struct sched_task) * TASK_MAX_COUNT);
    task_test();
}

/*
 * Api - Create new task
 */
extern bool task_create(u_short tid, void *address) {
    kprint(MSG_SCHED_TID_CREATE, (u_int)address);

    struct sched_task *task;
    int index;

    /* get free task entry */
    index = task_get_free_index();
    /* check task has allocated */
    if (index == -1) {
        kprint(MSG_SCHED_TID_EXCEED);
        return false;
    }
    /* deny tid duplicates */
    if (task_find_index(tid) != -1) {
        kprint(MSG_SCHED_TID_EXISTS);
        return false;
    }
    /* get task */
    task = &tasks[index];
    /* fill data */
    task->tid = tid;
    task->is_valid = true;
    task->status = TASK_UNINTERRUPTABLE;
    task->msg_count_in = 0;
    task->time = 0;
    /* set flags */
    *(u32*)(&task->flags) = asm_get_flags();
    /* set general purpose registers */
    memset(&task->gp_registers, 0, sizeof(struct gp_registers_t));
    /* set other purpose registers */
    task->op_registers.cs = asm_get_cs();
    task->op_registers.ds = asm_get_ds();
    task->op_registers.ss = asm_get_ss();
    task->op_registers.eip = (size_t)address;
    task->op_registers.esp = (size_t)&stacks[index] + TASK_STACK_SIZE;
    
    return true;
}

/*
 * Api - Get task by id
 */
extern struct sched_task *task_get_by_id(u_short tid) {
    struct sched_task *task;
    int index;

    index = task_find_index(tid);
    task = task_get_by_index(index);

    return task;
}

/*
 * Api - Get task by index
 */
extern struct sched_task *task_get_by_index(int index) {
    struct sched_task *task;

    kassert(__FILE__, __LINE__, index < TASK_MAX_COUNT);
    task = &tasks[index];
    kassert(__FILE__, __LINE__, task != null);

    return task;
}

/*
 * Api - Set task status by id
 */
extern bool task_set_status_by_id(u_short tid, u_short status) {
    struct sched_task *task;

    task = task_get_by_id(tid);
    task->status = status;

    return true;
}

/*
 * Api - Find first task to run from index
 */
extern int task_find_to_run_index(int index) {
    struct sched_task *task;
    int i;

    /* find after specified index */
    for (i = index + 1; i < TASK_MAX_COUNT; ++i) {
        task = &tasks[i];
        if (task->is_valid && task->status == TASK_RUNNING) {
            return i;
        }
    }

    /* find with first index */
    for (i = 0; i < index + 1; ++i) {
        task = &tasks[i];
        if (task->is_valid && task->status == TASK_RUNNING) {
            return i;
        }
    }

    return -1;
}

/*
 * Api - Find task by id
 */
extern int task_find_index(u_short tid) {
    int i;

    for (i = 0; i < TASK_MAX_COUNT; ++i) {
        if (tasks[i].tid == tid && tasks[i].is_valid) {
            return i;
        }
    }

    return -1;
}

/*
 * Api - Pack message
 */
extern void task_pack_message(u_short tid, struct message_t *msg) {
    struct sched_task *task;

    /* get target task */
    task = task_get_by_id(tid);
    /* check buffer size */
    if (task->msg_count_in == TASK_MSG_BUFF_SIZE) {
        kpanic(MSG_KERNEL_TASK_BUFF_EXCEED, tid);
    }
    /* unshift message to task buffer */
    task->msg_count_in += 1;
    for (int i = task->msg_count_in - 1; i > 0; --i) {
        memcpy(&task->msg_buff[i], &task->msg_buff[i-1], sizeof(struct message_t));
    }
    /* unshift message to task buffer */
    memcpy(&task->msg_buff[0], msg, sizeof(struct message_t));
}

/*
 * Api - Extract message
 */
extern void task_extract_message(u_short tid, struct message_t *msg) {
    struct sched_task *task;
    struct message_t *cur_msg;

    /* get target task */
    task = task_get_by_id(tid);
    kassert(__FILE__, __LINE__, task->msg_count_in > 0);
    /* get first incomed message */
    cur_msg = &task->msg_buff[--task->msg_count_in];
    kassert(__FILE__, __LINE__, task->msg_count_in >= 0);
    /* copy message to result buffer */
    memcpy(msg, cur_msg, sizeof(struct message_t));
}

/*
 * Get first free task entry
 */
static int task_get_free_index() {
    int i;

    for (i = 0; i < TASK_MAX_COUNT; ++i) {
        if (!tasks[i].is_valid) {
            return i;
        }
    }

    return -1;
}

/*
 * Smoke test
 */
static void task_test() {
#ifdef TEST
    struct sched_task *task;
    struct sched_task task1;
    struct sched_task task2;
    struct message_t msg;
    struct message_t msg1;
    struct message_t msg2;

    task1.tid = 1;
    task2.tid = 1;

    msg1.type = 1;
    msg1.len = 0;
    msg2.type = 2;
    msg2.len = 0;

    /* tasks creation */
    task_create(task1.tid, &task1);
    task_create(task2.tid, &task2);
    task = task_get_by_id(task1.tid);
    kassert(__FILE__, __LINE__, task->tid == task1.tid);
    task = task_get_by_id(task2.tid);
    kassert(__FILE__, __LINE__, task->tid == task2.tid);

    /* messages */
    task_pack_message(task1.tid, &msg1);
    task_pack_message(task1.tid, &msg2);
    task_extract_message(task1.tid, &msg);
    kassert(__FILE__, __LINE__, msg.type == msg1.type);
    task_extract_message(task1.tid, &msg);
    kassert(__FILE__, __LINE__, msg.type == msg2.type);

    /* clear task table */
    memset(tasks, 0, sizeof(struct sched_task) * TASK_MAX_COUNT);
#endif
}
