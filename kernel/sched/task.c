#include <arch/idt.h>
#include <arch/reg.h>
#include <sched/task.h>
#include <ipc/ipc.h>
#include <utils/kprint.h>
#include <utils/kpanic.h>
#include <utils/kheap.h>
#include <lib/string.h>
#include <lib/stdtypes.h>
#include <assembly.h>
#include <messages.h>

static void task_test();

/*
 * Tasks
 */

static struct sched_task_t *task_list_head; /* cyclic list */
static int task_count; /* list entries count */

/*
 * Api - Init
 */
extern void task_init() {
    task_list_head = null;
    task_count = 0;
    task_test();
}

/*
 * Api - Create new task
 */
extern bool task_create(u_short tid, void *address) {
    struct sched_task_t *task;

    kprint(MSG_SCHED_TID_CREATE, (u_int)address);

    /* check tasks limit */
    if (task_count >= TASK_MAX_COUNT) {
        return false; /* limit exceed */
    }

    /* allocate memory */
    task = kmalloc(sizeof(struct sched_task_t));
    task->kstack = kmalloc(TASK_KSTACK_SIZE);
    task->ustack = kmalloc(TASK_USTACK_SIZE);
    task_count += 1;
    /* normalize pointers */
    if (task_list_head) {
        task->next = task_list_head->next;
        task_list_head->next->prev = task;
        task->prev = task_list_head;
        task_list_head->next = task;
    } else {
        task->prev = task;
        task->next = task;
    }
    task_list_head = task;
    /* fill data */
    task->tid = tid;
    task->is_valid = true;
    task->status = TASK_UNINTERRUPTABLE;
    task->msg_count_in = 0;
    task->time = 0;
    /* set flags */
    *(u32*)(&task->flags) = asm_get_flags();
    /* set general purpose registers */
    memset(&task->gp_registers, 0, sizeof(struct gp_registers_t));
    /* set other purpose registers */
    task->op_registers.cs = asm_get_cs();
    task->op_registers.ds = asm_get_ds();
    task->op_registers.ss = asm_get_ss();
    task->op_registers.eip = (size_t)address;
    task->op_registers.k_esp = (u32)task->kstack + TASK_KSTACK_SIZE;
    task->op_registers.u_esp = (u32)task->ustack + TASK_USTACK_SIZE;
    
    return true;
}

/*
 * Api - Delete task by id
 */
extern void task_delete(struct sched_task_t *task) {
    /* normalize list head */
    task_count -= 1;
    if (task_list_head == task) {
        task_list_head = task_list_head->next;
        if (task_list_head == task) {
            task_list_head = null;
        }
    }
    /* normalize pointers */
    task->next->prev = task->prev;
    task->prev->next = task->next;
    /* free memory */
    kfree(task->kstack);
    kfree(task->ustack);
    memset(task, 0, sizeof(struct sched_task_t));
    kfree(task);
}

/*
 * Api - Get task by id
 */
extern struct sched_task_t *task_get_by_id(u_short tid) {
    struct sched_task_t *task;
    
    task = task_find_by_id(tid);
    kassert(__FILE__, __LINE__, task != null);

    return task;
}

/*
 * Api - Find task by id
 */
extern struct sched_task_t *task_find_by_id(u_short tid) {
    struct sched_task_t *task;
    
    if (task_list_head == null) {
        return null;
    }

    task = task_list_head;

    do
    {
        if (task->tid == tid) {
            return task;
        }

        task = task->next;
    } while (task != task_list_head);

    return null;
}

/*
 * Api - Get task by status
 */
extern struct sched_task_t *task_get_by_status(u_short status, struct sched_task_t *list_head) {
    struct sched_task_t *task;
    
    task = list_head;
    kassert(__FILE__, __LINE__, task != null);
    
    do
    {
        if (task->status == status) {
            return task;
        }

        task = task->next;
    } while (task != task_list_head);

    kpanic(MSG_KERNEL_CODE_UNREACHABLE);
}

/*
 * Api - Pack message
 */
extern void task_pack_message(struct sched_task_t *task, struct message_t *msg) {
    /* check buffer size */
    if (task->msg_count_in == TASK_MSG_BUFF_SIZE) {
        kpanic(MSG_KERNEL_TASK_BUFF_EXCEED, task->tid);
    }
    /* unshift message to task buffer */
    task->msg_count_in += 1;
    for (int i = task->msg_count_in - 1; i > 0; --i) {
        memcpy(&task->msg_buff[i], &task->msg_buff[i-1], sizeof(struct message_t));
    }
    /* unshift message to task buffer */
    memcpy(&task->msg_buff[0], msg, sizeof(struct message_t));
}

/*
 * Api - Extract message
 */
extern void task_extract_message(struct sched_task_t *task, struct message_t *msg) {
    struct message_t *cur_msg;

    /* get first incomed message */
    kassert(__FILE__, __LINE__, task->msg_count_in > 0);
    cur_msg = &task->msg_buff[--task->msg_count_in];
    kassert(__FILE__, __LINE__, task->msg_count_in >= 0);
    /* copy message to result buffer */
    memcpy(msg, cur_msg, sizeof(struct message_t));
}

/*
 * Smoke test
 */
static void task_test() {
#ifdef TEST
    struct sched_task_t *task;
    struct sched_task_t task1;
    struct sched_task_t task2;
    struct message_t msg;
    struct message_t msg1;
    struct message_t msg2;

    task1.tid = 1;
    task2.tid = 1;

    msg1.type = 1;
    msg1.len = 0;
    msg2.type = 2;
    msg2.len = 0;

    /* tasks creation */
    kassert(__FILE__, __LINE__, task_count == 0);
    kassert(__FILE__, __LINE__, task_list_head == null);
    task_create(task1.tid, &task1);
    task_create(task2.tid, &task2);
    task = task_get_by_id(task1.tid);
    kassert(__FILE__, __LINE__, task->tid == task1.tid);
    task = task_get_by_id(task2.tid);
    kassert(__FILE__, __LINE__, task->tid == task2.tid);
    kassert(__FILE__, __LINE__, task_count == 2);

    /* messages */
    kassert(__FILE__, __LINE__, task1.msg_count_in == 0);
    task_pack_message(&task1, &msg1);
    task_pack_message(&task1, &msg2);
    kassert(__FILE__, __LINE__, task1.msg_count_in == 2);
    task_extract_message(&task1, &msg);
    kassert(__FILE__, __LINE__, task1.msg_count_in == 1);
    kassert(__FILE__, __LINE__, msg.type == msg1.type);
    task_extract_message(&task1, &msg);
    kassert(__FILE__, __LINE__, task1.msg_count_in == 0);
    kassert(__FILE__, __LINE__, msg.type == msg2.type);

    /* task deletion */
    task_delete(&task1);
    task_delete(&task2);
    kassert(__FILE__, __LINE__, task_count == 0);
    kassert(__FILE__, __LINE__, task_list_head == null);
#endif
}
