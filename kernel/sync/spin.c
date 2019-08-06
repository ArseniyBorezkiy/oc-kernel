#include <arch/idt.h>
#include <sync/spin.h>
#include <sched/sched.h>
#include <utils/kpanic.h>
#include <lib/time.h>
#include <messages.h>

/*
 * Api - lock spin
 *   This function has blocking behaviour
 */
extern void spin_lock(struct spin_t *spin) {
    struct sched_task_t *task;

    task = sched_get_current_task();

    while (1) {
        /* whether spin is free */
        if (spin->semaphore == 0) {
            /* try to lock spin */
            asm_lock();

            if (spin->semaphore > 0) {
                /* too late */
                asm_unlock();
                sched_yield();
                continue;
            }
    
            /* do lock */
            spin->tid = task->tid;
            spin->semaphore += 1;

            asm_unlock();
        }
        /* or we locked it before */
        else if (spin->tid == task->tid) {
            /* lock again */
            spin->semaphore += 1;
        }
    }
}

/*
 * Api - unlock spin
 */
extern void spin_unlock(struct spin_t *spin) {
    struct sched_task_t *task;

    task = sched_get_current_task();

    if (spin->semaphore > 0 && spin->tid == task->tid) {
        /* release spin */
        asm_lock();
        spin->semaphore -= 1;
        if (spin->semaphore == 0) {
            /* totally released */
            spin->tid = 0;
        }
        asm_unlock();
    } else {
        /* unpair unlock */
        kunreachable(__FILE__, __LINE__);
    }
}
