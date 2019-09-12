#pragma once

#include <dev/utils/ih_low.h>
#include <lib/data/clist.h>
#include <lib/stdio.h>
#include <lib/stdtypes.h>

/*
 * Device definition
 */

typedef u_int (*dev_read_cb_t)(struct io_buf_t* io_buf, void* buff, u_int size);
typedef void (*dev_write_cb_t)(struct io_buf_t* io_buf, void* data, u_int size);
typedef void (*dev_ioctl_cb_t)(struct io_buf_t* io_buf, int command);

extern struct dev_t {
    struct clist_head_t list_head; /* should be at first */
    char name[8]; /* device name */
    void* base_r; /* base read address */
    void* base_w; /* base write address */
    dev_read_cb_t read_cb; /* read handler */
    dev_write_cb_t write_cb; /* write handler */
    dev_ioctl_cb_t ioctl_cb; /* device specific command handler */
    struct clist_definition_t ih_list; /* low half interrupt handlers */
};

typedef void (*dev_each_callback_t)(struct dev_t* entry, void* data);

/*
 * Api
 */
extern void dev_init();
extern struct clist_definition_t *dev_get_dev_list();
extern void dev_register(struct dev_t* dev);
extern struct dev_t* dev_find_by_name(char* name);
extern void dev_for_each(dev_each_callback_t callback, void* data);
