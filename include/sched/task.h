#pragma once

#include <types.h>

#define MAX_TASKS_COUNT 16
#define TASK_STACK_SIZE (4 * 1024)

#define TID_INIT 1
#define TID_TTY 2

/*
 * Process descriptor
 */
struct sched_task {
    u_short tid; /* task id */
    u_char is_valid; /* whether entry is valid task */
    u_char is_running; /* whether task is currently running */
    u_int registers[8]; /* general purpose registers */
    u_int flags; /* processor flags */
    u_int pc; /* program counter */
    u_int sp; /* stack pointer */
    u_short cs; /* code selector */
    u_short ds; /* data selector */
    u_short ss; /* stack selector */
    u_int time; /* time of task execution */
};

/*
 * Api
 */
extern void sched_init();
extern bool sched_create_task(u_short tid, void *address);
extern struct sched_task *get_task_by_index(int index);
extern bool sched_run_task_by_id(u_short tid);
extern bool sched_stop_task_by_id(u_short tid);
extern int sched_find_task_to_run_index(int index);
