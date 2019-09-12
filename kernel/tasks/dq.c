#include <sched/task.h>
#include <ipc/ipc.h>
#include <kernel.h>
#include <lib/sys.h>
#include <lib/stdio.h>
#include <lib/assert.h>
#include <tasks/dq.h>
#include <messages.h>

char* dq_task_name = "dq";

/*
 * Deferred queue execution scheduler
 * This task running in kernel mode
 */
void dq_task() {
    struct message_t msg;

    for (;;) {
        kreceive(TID_DQ, &msg);

        switch (msg.type) {
            case IPC_MSG_TYPE_DQ_SCHED:
                /* do deffered callback execution */
                assert(msg.len == 4);
                dq_handler_t handler = (dq_handler_t)*((size_t*)msg.data);
                assert((size_t)handler < KERNEL_CODE_END_ADDR);
                printf(MSG_DQ_SCHED, handler);
                handler(msg);
                break;
        }
    }

    exit(0);
}
