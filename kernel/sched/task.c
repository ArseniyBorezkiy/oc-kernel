#include <sched/task.h>
#include <arch/memory.h>
#include <arch/idt.h>
#include <arch/reg.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <messages.h>
#include <types.h>

static int sched_find_task_index(u_short tid);
static int sched_get_free_task_index();

/*
 * Tasks
 */
static struct sched_task tasks[MAX_TASKS_COUNT];
static void *stacks[TASK_STACK_SIZE][MAX_TASKS_COUNT];

/*
 * Api - Create new task
 */
extern bool sched_create_task(u_short tid, void *address) {
    struct sched_task *task;
    int index;

    index = sched_get_free_task_index();

    /* check task has allocated */
    if (index == -1) {
        kprint(MSG_SCHED_TID_EXCEED);
        return false;
    }

    /* deny tid duplicates */
    if (sched_find_task_index(tid) != -1) {
        kprint(MSG_SCHED_TID_EXISTS);
        return false;
    }

    task = &tasks[index];

    /* fill data */
    task->tid = tid;
    task->is_valid = true;
    task->is_running = false;
    task->time = 0;
    task->pc = (size_t)address;
    task->flags = asm_get_flags();
    task->cs = asm_get_cs();
    task->ds = asm_get_ds();
    task->ss = asm_get_ss();
    task->sp = (size_t)&stacks[index] + sizeof(struct sched_task);

    return true;
}

/*
 * Api - Get task by index
 */
extern struct sched_task *get_task_by_index(int index) {
  return &tasks[index];
}

/*
 * Api - Run task by id
 */
extern bool sched_run_task_by_id(u_short tid) {
    struct sched_task *task;
    int index;

    index = sched_find_task_index(tid);

    /* check task found */
    if (index == -1) {
        kprint(MSG_SCHED_TID_UNKNOWN);
        return false;
    }

    task = &tasks[index];
    task->is_running = true;

    return true;
}

/*
 * Api - Stop task by id
 */
extern bool sched_stop_task_by_id(u_short tid) {
    struct sched_task *task;
    int index;

    index = sched_find_task_index(tid);

    /* check task found */
    if (index == -1) {
        kprint(MSG_SCHED_TID_UNKNOWN);
        return false;
    }

    task = &tasks[index];
    task->is_running = false;

    return true;
}

/*
 * Api - Find first task to run from index
 */
extern int sched_find_task_to_run_index(int index) {
    struct sched_task *task;
    int i;

    /* find after specified index */
    for (i = index + 1; i < MAX_TASKS_COUNT; ++i) {
        task = &tasks[i];
        if (task->is_valid && task->is_running) {
            return i;
        }
    }

    /* find with first index */
    for (i = 0; i < index + 1; ++i) {
        task = &tasks[i];
        if (task->is_valid && task->is_running) {
            return i;
        }
    }

    return -1;
}

/*
 * Find task by id
 */
static int sched_find_task_index(u_short tid) {
    int i;

    for (i = 0; i < MAX_TASKS_COUNT; ++i) {
        if (tasks[i].tid == tid && tasks[i].is_valid) {
            return i;
        }
    }

    return -1;
}

/*
 * Get first free task entry
 */
static int sched_get_free_task_index() {
    int i;

    for (i = 0; i < MAX_TASKS_COUNT; ++i) {
        if (!tasks[i].is_valid) {
            return i;
        }
    }

    return -1;
}
