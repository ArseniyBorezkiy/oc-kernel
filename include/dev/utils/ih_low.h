#pragma once

#include <lib/data/clist.h>

/*
 * Low half interrupt handler
 */

extern struct ih_low_data_t {
    int number; /* interrupt number */
    void* data; /* arguments */
};

typedef void (*ih_low_cb_t)(int number, struct ih_low_data_t* data);

extern struct ih_low_t {
    struct clist_head_t list_head; /* should be at first */
    int number; /* interrupt number */
    ih_low_cb_t handler; /* interrupt handler */
};
