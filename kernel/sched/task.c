#include <arch/arch.h>
#include <ipc/ipc.h>
#include <lib/assembly.h>
#include <lib/assert.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/stdtypes.h>
#include <lib/string.h>
#include <messages.h>
#include <mm/mm.h>
#include <sched/task.h>

static bool task_by_id_detector(struct clist_head_t* current, va_list args);
static bool task_by_status_detector(struct clist_head_t* current, va_list args);
static void task_test();

/*
 * Tasks
 */

struct clist_definition_t task_list = {
    .head = null,
    .slot_size = sizeof(struct task_t)
};

/*
 * Api - Init
 */
extern void task_init()
{
    task_list.head = null;
    task_test();
}

/*
 * Api - Task list
 */
extern struct clist_definition_t *task_get_task_list() {
    return &task_list;
}

/*
 * Api - Create new task
 */
extern bool task_create(u_short tid, void* address, struct task_mem_t *task_mem)
{
    struct task_t* task;
    struct clist_head_t* entry;

    /* allocate memory */
    entry = clist_insert_entry_after(&task_list, task_list.head);
    task = (struct task_t*)entry->data;
    task->kstack = malloc(TASK_KSTACK_SIZE);
    task->ustack = malloc(TASK_USTACK_SIZE);
    /* fill data */
    task->tid = tid;
    task->name[0] = '\0';
    task->status = TASK_UNINTERRUPTABLE;
    task->msg_count_in = 0;
    task->time = 0;
    memcpy(&task->task_mem, task_mem, sizeof(struct task_mem_t));

    /* set flags */
    *(u32*)(&task->flags) = asm_get_eflags() | 0x200;
    /* set general purpose registers */
    memset(&task->gp_registers, 0, sizeof(struct gp_registers_t));
    /* set other purpose registers */
    task->op_registers.cs = GDT_KCODE_SELECTOR;
    task->op_registers.ds = GDT_KDATA_SELECTOR;
    task->op_registers.ss = GDT_KSTACK_SELECTOR;
    task->op_registers.eip = (size_t)address;
    task->op_registers.cr3 = (size_t)task_mem->page_dir;
    task->op_registers.k_esp = (u32)task->kstack + TASK_KSTACK_SIZE;
    task->op_registers.u_esp = (u32)task->ustack + TASK_USTACK_SIZE;

    printf(MSG_SCHED_TID_CREATE, tid, (u_int)address, task->kstack, task->ustack);

    return true;
}

/*
 * Api - Delete task by id
 */
extern void task_delete(struct task_t* task)
{
    printf(MSG_SCHED_TID_DELETE, (u_int)task->tid);
    assert(task != null);

    /* free stack memory */
    free(task->kstack);
    free(task->ustack);
    task->kstack = null;
    task->ustack = null;
    
    /* free user pages memory */
    if (task->task_mem.pages_count > 0) {
        mm_phys_free_pages(task->task_mem.pages, task->task_mem.pages_count);
        task->task_mem.pages = null;
        task->task_mem.pages_count = 0;
    }
    /* clear resources */
    if (task->task_mem.page_dir != null) {
      mmu_destroy_user_page_directory(task->task_mem.page_dir, task->task_mem.page_table);
    }

    clist_delete_entry(&task_list, (struct clist_head_t*)task);
}

/*
 * Api - Get task by id
 */
extern struct task_t* task_get_by_id(u_short tid)
{
    struct task_t* task;

    task = task_find_by_id(tid);
    assert(task != null);

    return task;
}

/*
 * Api - Find task by id
 */
extern struct task_t* task_find_by_id(u_short tid)
{
    struct clist_head_t* current;
    current = clist_find(&task_list, task_by_id_detector, tid);
    if (current != null) {
        return (struct task_t*)current->data;
    }

    return null;
}

/*
 * Api - Get task by status
 */
extern struct task_t* task_get_by_status(u_short status)
{
    struct task_t* task;

    task = task_find_by_status(status);
    assert(task != null);

    return task;
}

/*
 * Api - Find task by status
 */
extern struct task_t* task_find_by_status(u_short status)
{
    struct clist_head_t* current;

    current = clist_find(&task_list, task_by_status_detector, status);

    if (current != null) {
        return (struct task_t*)current->data;
    } else {
        return null;
    }
}

/*
 * Api - Get next task by status
 */
extern struct task_t* task_get_next_by_status(u_short status, struct task_t* pos)
{
    struct clist_head_t* current;
    current = clist_find_next(&task_list, (struct clist_head_t*)pos, task_by_status_detector, status);
    assert(current != null);
    return (struct task_t*)current->data;
}

/*
 * Api - Pack message
 */
extern void task_pack_message(struct task_t* task, struct message_t* msg)
{
    /* check buffer size */
    if (task->msg_count_in == TASK_MSG_BUFF_SIZE) {
        abort(MSG_KERNEL_TASK_BUFF_EXCEED, task->tid);
    }
    /* unshift message to task buffer */
    task->msg_count_in += 1;
    for (int i = task->msg_count_in - 1; i > 0; --i) {
        memcpy(&task->msg_buff[i], &task->msg_buff[i - 1], sizeof(struct message_t));
    }
    /* unshift message to task buffer */
    memcpy(&task->msg_buff[0], msg, sizeof(struct message_t));
}

/*
 * Api - Extract message
 */
extern void task_extract_message(struct task_t* task, struct message_t* msg)
{
    struct message_t* cur_msg;

    /* get first incomed message */
    assert(task->msg_count_in > 0);
    cur_msg = &task->msg_buff[--task->msg_count_in];
    assert(task->msg_count_in >= 0);
    /* copy message to result buffer */
    memcpy(msg, cur_msg, sizeof(struct message_t));
}

/*
 * Helper to find task by id
 */
static bool task_by_id_detector(struct clist_head_t* current, va_list args)
{
    u_short tid = va_arg(args, u_short);
    struct task_t* task = (struct task_t*)current->data;
    return task->tid == tid;
}

/*
 * Helper to find task by status
 */
static bool task_by_status_detector(struct clist_head_t* current, va_list args)
{
    u_short status = va_arg(args, u_short);
    struct task_t* task = (struct task_t*)current->data;
    return task->status == status;
}

/*
 * Api - Task list dump
 */
extern void task_dump()
{
    printf("-- task list dump\n");

    struct clist_head_t* current;
    struct task_t* task;

    for (current = task_list.head; current != null; current = current->next) {
        task = (struct task_t*)current->data;
        printf("  tid=%u status=%u this=%X prev=%X next=%X\n", task->tid, task->status, current, current->prev, current->next);

        if (current->next == task_list.head) {
            break;
        }
    }
}

/*
 * Api - For each task
 */
extern void task_for_each(task_each_callback_t callback)
{
    struct clist_head_t* current;
    struct task_t* task;

    for (current = task_list.head; current != null; current = current->next) {
        task = (struct task_t*)current->data;
        callback(task);

        if (current->next == task_list.head) {
            break;
        }
    }
}

/*
 * Smoke test
 */
static void task_test()
{
#ifdef TEST
    struct task_t* task1;
    struct task_t* task2;
    struct task_mem_t task_mem;
    struct message_t msg;
    struct message_t msg1;
    struct message_t msg2;

    u_short tid1 = 1;
    u_short tid2 = 2;

    msg1.type = 1;
    msg1.len = 0;
    msg2.type = 2;
    msg2.len = 0;
    
    memset(&task_mem, 0, sizeof(struct task_mem_t));

    /* tasks creation */
    assert(task_list.head == null);
    task_create(tid1, 0, &task_mem);
    task_create(tid2, 0, &task_mem);
    task1 = task_get_by_id(tid1);
    assert(task1->tid == tid1);
    task2 = task_get_by_id(tid2);
    assert(task2->tid == tid2);

    /* messages */
    assert(task1->msg_count_in == 0);
    task_pack_message(task1, &msg1);
    task_pack_message(task1, &msg2);
    assert(task1->msg_count_in == 2);
    task_extract_message(task1, &msg);
    assert(task1->msg_count_in == 1);
    assert(msg.type == msg1.type);
    task_extract_message(task1, &msg);
    assert(task1->msg_count_in == 0);
    assert(msg.type == msg2.type);

    /* task deletion */
    task_delete(task1);
    task_delete(task2);
    assert(task_list.head == null);
#endif
}
