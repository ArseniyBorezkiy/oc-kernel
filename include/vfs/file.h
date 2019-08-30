#pragma once

#include <dev/dev.h>
#include <lib/data/clist.h>
#include <lib/stdio.h>
#include <lib/stdtypes.h>

/*
 * Abstract file definition
 */

extern struct file_t {
    struct clist_head_t list_head; /* should be at first */
    struct io_buf_t io_buf; /* file handler */
    char name[8]; /* file name */
    int mod_rw; /* whether read or write */
    struct dev_t* dev; /* whether device driver */
};

/*
 * Api 
 */
extern void file_init();
extern void file_register_device(struct dev_t* dev);
extern struct io_buf_t* file_open(char* path, int mod_rw);
extern void file_close(struct io_buf_t* io_buf);
extern size_t file_read(struct io_buf_t* io_buf, char* buff, u_int size);
extern void file_write(struct io_buf_t* io_buf, char* data, u_int size);
extern void file_ioctl(struct io_buf_t* io_buf, int command);
