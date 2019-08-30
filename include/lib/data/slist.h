#pragma once

/*
 * Static list implementation.
 * Located in fixed array.
 */

#include <lib/stdtypes.h>

struct slist_head_t {
    struct slist_head_t* prev;
    struct slist_head_t* next;
    bool is_valid;
    void* data;
} attribute(packed);

struct slist_definition_t {
    size_t base;
    u_int slots;
    size_t slot_size;
    struct slist_head_t* head;
    struct slist_head_t* tail;
};

typedef struct slist_head_t* (*slist_reduce_callback_t)(struct slist_head_t* current, struct slist_head_t* result);
typedef bool (*slist_find_callback_t)(struct slist_head_t* current);

/*
 * Api
 */
extern void slist_init();
extern struct slist_head_t* slist_insert_entry_after(struct slist_definition_t* list, struct slist_head_t* pos);
extern struct slist_head_t* slist_insert_entry_before(struct slist_definition_t* list, struct slist_head_t* pos);
extern void slist_delete_entry(struct slist_definition_t* list, struct slist_head_t* entry);
extern struct slist_head_t* slist_reduce(struct slist_definition_t* list, slist_reduce_callback_t reducer);
extern struct slist_head_t* slist_find(struct slist_definition_t* list, slist_find_callback_t detector);
extern void slist_dump(struct slist_head_t* head);
