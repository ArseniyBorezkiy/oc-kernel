#pragma once

#include <types.h>

#define TASK_QUOTA 18

/*
 * Api
 */
extern void sched_schedule(size_t *ret_addr, size_t *reg_addr);
