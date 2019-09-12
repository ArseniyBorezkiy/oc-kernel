#pragma once

#include <lib/stdtypes.h>

#define IPC_MSG_TYPE_DQ_SCHED 10

typedef void (*dq_handler_t)();

/*
 * Api
 */
void dq_task();

/*
 * Data
 */
extern char* dq_task_name;
