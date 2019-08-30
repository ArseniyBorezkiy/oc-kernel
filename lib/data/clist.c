#include <lib/assembly.h>
#include <lib/assert.h>
#include <lib/data/clist.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>

static void clist_test();

/*
 * Api - init cyclic list module
 */
extern void clist_init()
{
    clist_test();
}

/*
 * Api - Create list entry after position
 */
extern struct clist_head_t* clist_insert_entry_after(struct clist_definition_t* list, struct clist_head_t* pos)
{
    assert(pos != null || pos == list->head);

    /* allocate memory */
    struct clist_head_t* entry;
    size_t addr = (size_t)malloc(list->slot_size);
    entry = (struct clist_head_t*)addr;
    entry->data = (void*)addr;

    if (list->head == null) {
        /* insert first entry */
        entry->next = entry;
        entry->prev = entry;
        list->head = entry;
    } else {
        /* insert next entry between pos and pos next */
        struct clist_head_t* prev = pos;
        struct clist_head_t* next = pos->next;
        entry->prev = prev;
        entry->next = next;
        if (prev != null) {
            prev->next = entry;
        }
        if (next != null) {
            next->prev = entry;
        }
    }

    return entry;
}

/*
 * Api - Create list entry before position
 */
extern struct clist_head_t* clist_insert_entry_before(struct clist_definition_t* list, struct clist_head_t* pos)
{
    assert(pos != null || pos == list->head);

    /* allocate memory */
    struct clist_head_t* entry;
    size_t addr = (size_t)malloc(list->slot_size);
    entry = (struct clist_head_t*)addr;
    entry->data = (void*)addr;

    if (list->head == null) {
        /* insert first entry */
        entry->next = entry;
        entry->prev = entry;
        list->head = entry;
    } else {
        /* insert next entry between pos prev and pos */
        struct clist_head_t* prev = pos->prev;
        struct clist_head_t* next = pos;
        entry->prev = prev;
        entry->next = next;
        if (prev != null) {
            prev->next = entry;
        }
        if (next != null) {
            next->prev = entry;
        }
    }

    return entry;
}

/*
 * Api - Delete list entry
 */
extern void clist_delete_entry(struct clist_definition_t* list, struct clist_head_t* entry)
{
    assert(entry != null);
    assert(list->head != null);

    struct clist_head_t* prev = entry->prev;
    struct clist_head_t* next = entry->next;

    prev->next = next;
    next->prev = prev;

    if (list->head == entry) {
        list->head = entry->next != entry ? entry->next : null;
    }

    free(entry);
}

/*
 * Api - Find first suitable entry in cyclic list
 */
extern struct clist_head_t* clist_find(struct clist_definition_t* list, clist_find_callback_t detector, ...)
{
    struct clist_head_t* current;

    for (current = list->head; current != null; current = current->next) {
        va_list args;
        va_start(args, detector);

        if (detector(current, args)) {
            return current;
        }

        if (current->next == list->head) {
            break;
        }
    }

    return null;
}

/*
 * Api - Find first next suitable entry in cyclic list
 */
extern struct clist_head_t* clist_find_next(struct clist_definition_t* list, struct clist_head_t* pos, clist_find_callback_t detector, ...)
{
    struct clist_head_t* current;

    for (current = pos->next; current != null && current != pos; current = current->next) {
        va_list args;
        va_start(args, detector);

        if (detector(current, args)) {
            return current;
        }
    }

    if (pos != null) {
        va_list args;
        va_start(args, detector);

        if (detector(current, args)) {
            return current;
        }
    }

    return null;
}

/*
 * Api - Cyclic list for each
 */
extern void clist_for_each(struct clist_definition_t* list, clist_each_callback_t cb, ...) {
    struct clist_head_t* current;

    for (current = list->head; current != null; current = current->next) {
        va_list args;
        va_start(args, cb);

        cb(current, args);

        if (current->next == list->head) {
            break;
        }
    }
}

/*
 * Api - Cyclic list dump
 */
extern void clist_dump(struct clist_definition_t* list)
{
    printf("-- cyclic list dump\n");

    struct clist_head_t* current;

    for (current = list->head; current != null; current = current->next) {
        printf("  this=%X prev=%X next=%X\n", current, current->prev, current->next);
        if (current->next == list->head) {
            break;
        }
    }
}

/*
 * Smoke test
 */
static void clist_test()
{
#ifdef TEST
    struct clist_definition_t list = {
        .head = null,
        .slot_size = sizeof(struct clist_head_t)
    };

    struct clist_head_t* entry1;
    struct clist_head_t* entry2;
    struct clist_head_t* entry3;
    struct clist_head_t* entry4;

    /* create first entry (1) */
    entry1 = clist_insert_entry_after(&list, null);
    assert(entry1 != null);
    assert(list.head == entry1);
    /* create second entry (2, 1) */
    entry2 = clist_insert_entry_before(&list, entry1);
    assert(entry2 != null);
    assert(list.head == entry1);
    /* create third entry (2, 1, 3) */
    entry3 = clist_insert_entry_after(&list, entry1);
    assert(entry3 != null);
    assert(list.head == entry1);
    /* create fourth entry (2, 1, 3, 4) */
    entry4 = clist_insert_entry_after(&list, entry3);
    assert(entry4 != null);
    assert(list.head == entry1);

    /* delete first entry (2, 3, 4) */
    clist_delete_entry(&list, entry1);
    assert(list.head == entry3);
    /* delete fourth entry (2, 3) */
    clist_delete_entry(&list, entry4);
    assert(list.head == entry3);
    /* delete third entry (2) */
    clist_delete_entry(&list, entry3);
    assert(list.head == entry2);
    /* delete second entry () */
    clist_delete_entry(&list, entry2);
    assert(list.head == null);
#endif
}
