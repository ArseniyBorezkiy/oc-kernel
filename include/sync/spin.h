#pragma once

#include <lib/stdtypes.h>

struct spin_t {
    u_short tid;
    int semaphore;
};

/*
 * Api
 */
extern void spin_lock(struct spin_t* spin);
extern void spin_unlock(struct spin_t* spin);
