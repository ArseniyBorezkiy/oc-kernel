#pragma once

/*
 * Cyclic list implementation.
 * Located in kernel heap.
 */

#include <lib/stdtypes.h>

struct clist_head_t {
    struct clist_head_t* prev;
    struct clist_head_t* next;
    void* data;
} attribute(packed);

struct clist_definition_t {
    size_t slot_size;
    struct clist_head_t* head;
};

typedef bool (*clist_find_callback_t)(struct clist_head_t* current, va_list list);
typedef void (*clist_each_callback_t)(struct clist_head_t* current, va_list list);

/*
 * Api
 */
extern void clist_init();
extern struct clist_head_t* clist_insert_entry_after(struct clist_definition_t* list, struct clist_head_t* pos);
extern struct clist_head_t* clist_insert_entry_before(struct clist_definition_t* list, struct clist_head_t* pos);
extern void clist_delete_entry(struct clist_definition_t* list, struct clist_head_t* entry);
extern struct clist_head_t* clist_find(struct clist_definition_t* list, clist_find_callback_t detector, ...);
extern struct clist_head_t* clist_find_next(struct clist_definition_t* list, struct clist_head_t* pos, clist_find_callback_t detector, ...);
extern void clist_for_each(struct clist_definition_t* list, clist_each_callback_t cb, ...);
extern void clist_dump(struct clist_definition_t* list);
