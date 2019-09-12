#include <sched/task.h>
#include <ipc/ipc.h>
#include <kernel.h>
#include <lib/sys.h>
#include <lib/stdio.h>
#include <lib/assert.h>
#include <tasks/init.h>
#include <messages.h>
#include <vfs/initrd.h>

char* init_task_name = "init";

/*
 * Main user process
 * This task running in kernel mode
 */
void init_task() {
    initrd_autorun();
    exit(0);
}
