#include <lib/assembly.h>
#include <lib/assert.h>
#include <lib/data/slist.h>
#include <lib/stdio.h>
#include <lib/string.h>

static void slist_test();

/*
 * Api - Init static list
 */
extern void slist_init()
{
    slist_test();
}

/*
 * Api - Create static list entry after position
 */
extern struct slist_head_t* slist_insert_entry_after(struct slist_definition_t* list, struct slist_head_t* pos)
{
    struct slist_head_t* entry;

    assert(list != null);
    assert(list->slot_size >= sizeof(struct slist_head_t));

    for (int i = 0; i < list->slots; ++i) {
        size_t offset = list->base + i * list->slot_size;
        entry = (struct slist_head_t*)offset;

        if (!entry->is_valid) {
            /* occupy block */
            entry->is_valid = true;
            entry->data = (void*)offset;

            struct slist_head_t* prev = null;
            struct slist_head_t* next = null;
            if (pos == null) {
                prev = null;
                next = list->head;
            } else {
                prev = pos;
                next = pos->next;
            }

            /* insert to list */
            entry->prev = prev;
            entry->next = next;
            if (prev != null) {
                prev->next = entry;
            }
            if (next != null) {
                next->prev = entry;
            }

            /* normalize list tail */
            if (pos == list->tail) {
                list->tail = entry;
            }
            /* normalize list head */
            if (list->head == null || pos == null) {
                list->head = entry;
            }

            return entry;
        }
    }

    return null;
}

/*
 * Api - Create static list entry before position
 */
extern struct slist_head_t* slist_insert_entry_before(struct slist_definition_t* list, struct slist_head_t* pos)
{
    struct slist_head_t* entry;

    assert(list != null);
    assert(list->slot_size >= sizeof(struct slist_head_t));

    for (int i = 0; i < list->slots; ++i) {
        size_t offset = list->base + i * list->slot_size;
        entry = (struct slist_head_t*)offset;

        if (!entry->is_valid) {
            /* occupy block */
            entry->is_valid = true;
            entry->data = (void*)offset;

            struct slist_head_t* prev = null;
            struct slist_head_t* next = null;
            if (pos == null) {
                prev = null;
                next = list->head;
            } else {
                next = pos;
                prev = pos->prev;
            }

            /* insert to list */
            entry->prev = prev;
            entry->next = next;
            if (prev != null) {
                prev->next = entry;
            }
            if (next != null) {
                next->prev = entry;
            }

            /* normalize list head */
            if (pos == list->head || pos == null) {
                list->head = entry;
            }
            /* normalize list tail */
            if (list->tail == null) {
                list->tail = entry;
            }

            return entry;
        }
    }

    return null;
}

/*
 * Api - Delete static list element
 */
extern void slist_delete_entry(struct slist_definition_t* list, struct slist_head_t* entry)
{
    assert(list != null);
    assert(entry != null);

    struct slist_head_t* prev = entry->prev;
    struct slist_head_t* next = entry->next;

    /* delete from list */
    if (prev != null) {
        prev->next = next;
    }
    if (next != null) {
        next->prev = prev;
    }

    /* normalize list head and tail */
    if (entry == list->head) {
        list->head = next;
    }
    if (entry == list->tail) {
        list->tail = prev;
    }

    /* delete entry */
    memset(entry->data, 0, list->slot_size);
}

/*
 * Api - Reduce static list to single entry
 */
extern struct slist_head_t* slist_reduce(struct slist_definition_t* list, slist_reduce_callback_t reducer)
{
    struct slist_head_t* current = null;
    struct slist_head_t* result = null;

    for (current = list->head; current != null; current = current->next) {
        result = reducer(current, result);
    }

    return result;
}

/*
 * Api - Find first suitable entry in static list
 */
extern struct slist_head_t* slist_find(struct slist_definition_t* list, slist_find_callback_t detector)
{
    struct slist_head_t* current = null;

    for (current = list->head; current != null; current = current->next) {
        if (detector(current)) {
            return current;
        }
    }

    return null;
}

/*
 * Api - Static list dump
 */
extern void slist_dump(struct slist_head_t* head)
{
    printf("-- static list dump\n");

    struct slist_head_t* current;

    for (current = head; current != null; current = current->next) {
        printf("  this=%X prev=%X next=%X\n", current, current->prev, current->next);
    }
}

/*
 * Smoke test
 */
static void slist_test()
{
#ifdef TEST
    int size = 8;
    struct slist_head_t entries[size];
    struct slist_definition_t list = {
        .head = null,
        .tail = null,
        .slot_size = sizeof(struct slist_head_t),
        .slots = size,
        .base = (size_t)entries
    };

    struct slist_head_t* entry1;
    struct slist_head_t* entry2;
    struct slist_head_t* entry3;
    struct slist_head_t* entry4;

    /* init list */
    memset(entries, 0, sizeof(struct slist_head_t) * size);

    /* create first entry (1) */
    entry1 = slist_insert_entry_after(&list, null);
    assert(entry1 != null);
    assert(list.head == entry1);
    assert(list.tail == entry1);
    /* create second entry (2, 1) */
    entry2 = slist_insert_entry_before(&list, entry1);
    assert(entry2 != null);
    assert(list.head == entry2);
    assert(list.tail == entry1);
    /* create third entry (2, 1, 3) */
    entry3 = slist_insert_entry_after(&list, entry1);
    assert(entry3 != null);
    assert(list.head == entry2);
    assert(list.tail == entry3);
    /* create fourth entry (4, 2, 1, 3) */
    entry4 = slist_insert_entry_after(&list, null);
    assert(entry4 != null);
    assert(list.head == entry4);
    assert(list.tail == entry3);

    /* delete first entry (4, 2, 3) */
    slist_delete_entry(&list, entry1);
    assert(list.head == entry4);
    assert(list.tail == entry3);
    /* delete fourth entry (2, 3) */
    slist_delete_entry(&list, entry4);
    assert(list.head == entry2);
    assert(list.tail == entry3);
    /* delete third entry (2) */
    slist_delete_entry(&list, entry3);
    assert(list.head == entry2);
    assert(list.tail == entry2);
    /* delete second entry () */
    slist_delete_entry(&list, entry2);
    assert(list.head == null);
    assert(list.tail == null);
#endif
}
