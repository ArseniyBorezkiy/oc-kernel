#pragma once

#include <dev/dev.h>
#include <lib/stdtypes.h>
#include <lib/data/clist.h>
#include <lib/stdio.h>

/*
 * Abstract file definition
 */

extern struct file_t
{
  struct clist_head list_head; /* should be at first */
  struct io_buf_t io_buf;      /* file handler */
  char name[8];                /* file name */
  struct dev_t *dev;           /* whether device driver */
};

#define MOD_R 1
#define MOD_W 2

/*
 * Api
 */
extern void file_init();
extern void file_register_device(struct dev_t *dev);
extern struct file_t *file_open(char *path, int mod_rw);
extern void file_read(struct io_buf_t *io_buf, char *buff, u_int size);
extern void file_write(struct io_buf_t *io_buf, char *data, u_int size);
