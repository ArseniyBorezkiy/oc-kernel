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
void sched_create_task(u_short tid, void *address);
short int sched_find_task_index(u_short tid);
short int sched_get_free_task_index();
void sched_run_task_by_id(u_short tid);
void sched_stop_task_by_id(u_short tid);
unsigned int sched_find_task_to_run_index(u_short index);
void sched_schedule(size_t *task_ret_addr);
unsigned int asm_get_cs();
unsigned int asm_get_ds();
unsigned int asm_get_ss();
unsigned int asm_get_flags();
