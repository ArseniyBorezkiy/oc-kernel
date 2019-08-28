#pragma once

#include <lib/stdtypes.h>
#include <dev/utils/ih_low.h>
#include <lib/data/clist.h>

/*
 * Device definition
 */

typedef void (*dev_read_cb_t)(struct io_buf_t *io_buf, void *buff, u_int size);
typedef void (*dev_write_cb_t)(struct io_buf_t *io_buf, void *data, u_int size);
typedef void (*dev_ioctl_cb_t)(struct io_buf_t *io_buf, int command);

extern struct dev_t
{
  char name[8];                      /* device name */
  size_t base_r;                     /* base read address */
  size_t base_w;                     /* base write address */
  dev_read_cb_t read_cb;             /* read handler */
  dev_write_cb_t write_cb;           /* write handler */
  dev_ioctl_cb_t ioctl_cb;           /* device specific command handler */
  struct clist_definition_t ih_list; /* low half interrupt handlers */
};

typedef void (*dev_each_callback_t)(struct dev_t *entry, void *data);

/*
 * Api
 */
extern void dev_init();
extern void dev_register(struct dev_t *dev);
extern struct dev_t *dev_find_by_name(char *name);
extern void dev_for_each(dev_each_callback_t callback, void *data);
