#include <dev/utils/video.h>
#include <lib/assert.h>
#include <lib/string.h>
#include <vfs/file.h>

static bool file_list_by_name_detector(struct clist_head_t* current, va_list list);

/*
 * Data
 */
struct clist_definition_t file_list = {
    .head = null,
    .slot_size = sizeof(struct file_t)
};

static int next_fd = 1; /* fd allocator */

/*
 * Api - init module 
 */
extern void file_init()
{
    /* init data structures */
    file_list.head = null;
}

/*
 * Api - open file
 */
extern struct io_buf_t* file_open(char* path, int mod_rw)
{
    struct clist_head_t* entry;
    struct file_t* file;
    struct dev_t* dev;

    /* try to find already opened file */
    entry = clist_find(&file_list, file_list_by_name_detector, path, mod_rw);
    file = (struct file_t*)entry->data;
    if (entry != null) {
        return &file->io_buf;
    }

    /* create list entry */
    entry = clist_insert_entry_after(&file_list, file_list.head);
    file = (struct file_t*)entry->data;

    /* whether file is device */
    dev = dev_find_by_name(path);
    if (dev != null) {
        /* device */
        file->dev = dev;

        if (mod_rw == MOD_R) {
            file->io_buf.base = dev->base_r;
        } else if (mod_rw == MOD_W) {
            file->io_buf.base = dev->base_w;
        }
    } else {
        /* fs node */
        file->dev = null;
        unreachable(); /* fs in not implemented yet */
    }

    /* fill data */
    file->mod_rw = mod_rw;
    file->io_buf.fd = next_fd++;
    file->io_buf.ptr = file->io_buf.base;
    file->io_buf.is_eof = false;
    file->io_buf.is_eol = false;
    file->io_buf.file = file;
    strncpy(file->name, path, sizeof(file->name));

    return &file->io_buf;
}

/*
 * Api - close file
 */
extern void file_close(struct io_buf_t* io_buf)
{
    struct clist_head_t* entry;
    entry = io_buf->file;
    clist_delete_entry(&file_list, entry);
}

/*
 * Api - read from file
 */
extern size_t file_read(struct io_buf_t* io_buf, char* buff, u_int size)
{
    struct file_t* file;

    file = (struct file_t*)io_buf->file;

    /* whether file is device */
    if (file->dev != null) {
        /* device */
        return file->dev->read_cb(&file->io_buf, buff, size);
    } else {
        /* fs node */
        unreachable(); /* fs in not implemented yet */
    }

    return 0;
}

/*
 * Api - write to file
 */
extern void file_write(struct io_buf_t* io_buf, char* data, u_int size)
{
    struct file_t* file;

    file = (struct file_t*)io_buf->file;

    /* whether file is device */
    if (file->dev != null) {
        /* device */
        file->dev->write_cb(&file->io_buf, data, size);
    } else {
        /* fs node */
        unreachable(); /* fs in not implemented yet */
    }
}

/*
 * Api - specific file command
 */
extern void file_ioctl(struct io_buf_t* io_buf, int command)
{
    struct file_t* file;

    file = (struct file_t*)io_buf->file;

    /* whether file is device */
    if (file->dev != null) {
        /* device */
        file->dev->ioctl_cb(&file->io_buf, command);
    } else {
        /* fs node */
        unreachable(); /* fs in not implemented yet */
    }
}

/*
 * Find file by name
 */
static bool file_list_by_name_detector(struct clist_head_t* current, va_list list)
{
    char* name = va_arg(list, char*);
    int mod_rw = va_arg(list, int);
    struct file_t* file = (struct file_t*)current->data;
    return mod_rw == file->mod_rw && !strcmp(name, file->name);
}
