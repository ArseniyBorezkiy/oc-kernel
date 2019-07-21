#include <sched/task.h>
#include <arch/memory.h>
#include <arch/idt.h>
#include <utils/kprint.h>
#include <utils/panic.h>
#include <messages.h>
#include <types.h>

/*
 * Tasks
 */
struct sched_task tasks[MAX_TASKS_COUNT];
void *stacks[TASK_STACK_SIZE][MAX_TASKS_COUNT];
unsigned short current_index = -1;

/*
 * Create new task
 */
void sched_create_task(unsigned short tid, void *address) {
    struct sched_task *task;
    unsigned short index;

    index = sched_get_free_task_index();

    /* check task has allocated */
    if (index == -1) {
        kprint(MSG_SCHED_TID_EXCEED);
    }

    /* deny tid duplicates */
    if (sched_find_task_index(tid) != -1) {
        kprint(MSG_SCHED_TID_EXISTS);
    }

    task = &tasks[index];

    /* fill data */
    task->tid = tid;
    task->is_valid = true;
    task->is_running = false;
    task->time = 0;
    task->pc = (unsigned long)address;
    task->flags = asm_get_flags();
    task->cs = asm_get_cs();
    task->ds = asm_get_ds();
    task->ss = asm_get_ss();
    task->sp = (unsigned long)&stacks[index] + sizeof(struct sched_task);
}

/*
 * Run task by id
 */
void sched_run_task_by_id(unsigned short tid) {
    struct sched_task *task;
    unsigned short index;

    index = sched_find_task_index(tid);

    /* check task found */
    if (index == -1) {
        kprint(MSG_SCHED_TID_UNKNOWN);
    }

    task = &tasks[index];
    task->is_running = true;
}

/*
 * Stop task by id
 */
void sched_stop_task_by_id(unsigned short tid) {
    struct sched_task *task;
    unsigned short index;

    index = sched_find_task_index(tid);

    /* check task found */
    if (index == -1) {
        kprint(MSG_SCHED_TID_UNKNOWN);
    }

    task = &tasks[index];
    task->is_running = false;
}

/*
 * Schedule task to run
 */
void sched_schedule(unsigned long *task_ret_addr) {
    current_index = sched_find_task_to_run_index(current_index);

    /* check task found */
    if (current_index == -1) {
        panic(MSG_SCHED_NO_TASKS);
    }
}

/*
 * Find task by id
 */
short int sched_find_task_index(unsigned short tid) {
    unsigned int i;

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
short int sched_get_free_task_index() {
    unsigned int i;

    for (i = 0; i < MAX_TASKS_COUNT; ++i) {
        if (tasks[i].is_valid) {
            return i;
        }
    }

    return -1;
}

/*
 * Find first task to run from index
 */
unsigned int sched_find_task_to_run_index(unsigned short index) {
    struct sched_task *task;
    unsigned int i;

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