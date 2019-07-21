#pragma once

#define MAX_TASKS_COUNT 16
#define TASK_STACK_SIZE (4 * 1024)

#define TID_INIT 1
#define TID_TTY 2

/*
 * Process descriptor
 */
struct sched_task {
    unsigned short tid; /* task id */
    unsigned char is_valid; /* whether entry is valid task */
    unsigned char is_running; /* whether task is currently running */
    unsigned int registers[8]; /* general purpose registers */
    unsigned int flags; /* processor flags */
    unsigned int pc; /* program counter */
    unsigned int sp; /* stack pointer */
    unsigned short cs; /* code selector */
    unsigned short ds; /* data selector */
    unsigned short ss; /* stack selector */
    unsigned int time; /* time of task execution */
};

/*
 * Api
 */
void sched_create_task(unsigned short tid, void *address);
short int sched_find_task_index(unsigned short tid);
short int sched_get_free_task_index();
void sched_run_task_by_id(unsigned short tid);
void sched_stop_task_by_id(unsigned short tid);
unsigned int sched_find_task_to_run_index(unsigned short index);
void sched_schedule(unsigned long *task_ret_addr);
unsigned int asm_get_cs();
unsigned int asm_get_ds();
unsigned int asm_get_ss();
unsigned int asm_get_flags();
