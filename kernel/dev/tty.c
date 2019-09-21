#include <arch/arch.h>
#include <dev/dev.h>
#include <dev/tty.h>
#include <dev/utils/keyboard.h>
#include <dev/utils/video.h>
#include <ipc/ipc.h>
#include <tasks/dq.h>
#include <lib/assert.h>
#include <lib/data/clist.h>
#include <lib/stdio.h>
#include <lib/stdtypes.h>
#include <lib/string.h>
#include <lib/time.h>
#include <messages.h>
#include <sched/task.h>
#include <utils/kprint.h>

static void tty_write(struct io_buf_t* io_buf, void* data, u_int size);
static u_int tty_read(struct io_buf_t* io_buf, void* buffer, u_int size);
static void tty_ioctl(struct io_buf_t* io_buf, int command);
static void tty_write_ch(struct io_buf_t* io_buf, char ch);
static char tty_read_ch(struct io_buf_t* io_buf);
static void tty_keyboard_ih_low(int number, struct ih_low_data_t* data);
static void tty_keyboard_ih_high(struct message_t *msg);

/*
 * Data
 */
static const char* tty_dev_name = TTY_DEV_NAME; /* teletype device name */
static char tty_output_buff[VIDEO_SCREEN_SIZE]; /* teletype output buffer */
static char tty_input_buff[VIDEO_SCREEN_WIDTH]; /* teletype input buffer */
char* tty_output_buff_ptr = tty_output_buff;
char* tty_input_buff_ptr = tty_input_buff;
bool read_line_mode = false; /* whether read only whole line */
bool is_echo = false; /* whether to put readed symbol to stdout */

/*
 * Api - Teletype init
 */
extern void tty_init()
{
    struct clist_head_t* entry;
    struct dev_t dev;
    struct ih_low_t* ih_low;

    memset(tty_output_buff, 0, sizeof(VIDEO_SCREEN_SIZE));
    memset(tty_input_buff, 0, sizeof(VIDEO_SCREEN_WIDTH));

    /* register teletype device */
    strcpy(dev.name, tty_dev_name);
    dev.base_r = tty_input_buff;
    dev.base_w = tty_output_buff;
    dev.read_cb = tty_read;
    dev.write_cb = tty_write;
    dev.ioctl_cb = tty_ioctl;
    dev.ih_list.head = null;

    /* add interrupt handlers */
    dev.ih_list.slot_size = sizeof(struct ih_low_t);
    entry = clist_insert_entry_after(&dev.ih_list, dev.ih_list.head);
    ih_low = (struct ih_low_t*)entry->data;
    ih_low->number = INT_KEYBOARD;
    ih_low->handler = tty_keyboard_ih_low;

    dev_register(&dev);
}

/*
 * Key press low half handler
 */
static void tty_keyboard_ih_low(int number, struct ih_low_data_t* data)
{
    /* write character to input buffer */
    char* keycode = data->data;
    int index = *keycode;
    assert(index < 128);
    char ch = keyboard_map[index];

    if (ch != '\b' || !read_line_mode) {
        /* store character to keyboard buffer */
        *tty_input_buff_ptr++ = ch;
    }

    if (is_echo && ch != '\n' && ch != '\b') {
        /* echo usual character to screen */
        *tty_output_buff_ptr++ = ch;
    }
    
    if (read_line_mode && ch == '\b') {
        /* erase previous character */
        if (tty_input_buff_ptr > tty_input_buff) {
            *--tty_input_buff_ptr = '\0';
            *--tty_output_buff_ptr = ' ';
        }
    }

    /* register deffered execution */
    struct message_t msg;
    msg.type = IPC_MSG_TYPE_DQ_SCHED;
    msg.len = 4;
    *((size_t *)msg.data) = (size_t)tty_keyboard_ih_high;
    ksend(TID_DQ, &msg);
}

/*
 * Key press high half handler
 */
static void tty_keyboard_ih_high(struct message_t *msg)
{
    video_flush(tty_output_buff);
}

/*
 * Write to tty
 */
static void tty_write(struct io_buf_t* io_buf, void* data, u_int size)
{
    char* ptr = data;

    for (int i = 0; i < size && !io_buf->is_eof; ++i) {
        tty_write_ch(io_buf, *ptr++);
    }
}

/*
 * Read line from tty to string
 */
static u_int tty_read(struct io_buf_t* io_buf, void* buffer, u_int size)
{
    char* ptr = buffer;

    do {
        char ch = tty_read_ch(io_buf);

        if (ch != '\0') {
            *ptr++ = ch;
            size -= 1;
        }
    } while (size > 0 && (read_line_mode ? !io_buf->is_eol && !io_buf->is_eof : !io_buf->is_eof));

    return (size_t)ptr - (size_t)buffer;
}

/*
 * Teletype specific command
 */
static void tty_ioctl(struct io_buf_t* io_buf, int command)
{
    char* hello_msg = MSG_KERNEL_NAME;

    switch (command) {
    case IOCTL_INIT: /* prepare video device */
        if (io_buf->base == tty_output_buff) {
            kmode(false); /* detach syslog from screen */
            tty_output_buff_ptr = video_clear(io_buf->base);
            io_buf->ptr = tty_output_buff_ptr;
            tty_write(io_buf, hello_msg, strlen(hello_msg));
            video_flush(io_buf->base);
            io_buf->ptr = tty_output_buff_ptr;
        } else if (io_buf->base == tty_input_buff) {
            unreachable();
        }
        break;
    case IOCTL_CLEAR:
        if (io_buf->base == tty_output_buff) {
            /* fill output buffer with spaces */
            tty_output_buff_ptr = video_clear(io_buf->base);
            video_flush(io_buf->base);
            io_buf->ptr = tty_output_buff_ptr;
        } else if (io_buf->base == tty_input_buff) {
            /* clear input buffer */
            tty_input_buff_ptr = tty_input_buff;
            io_buf->ptr = io_buf->base;
            io_buf->is_eof = true;
        }
        break;
    case IOCTL_FLUSH: /* flush buffer to screen */
        if (io_buf->base == tty_output_buff) {
            video_flush(io_buf->base);
        } else if (io_buf->base == tty_input_buff) {
            unreachable();
        }
        break;
    case IOCTL_READ_MODE_LINE: /* read only whole line */
        if (io_buf->base == tty_input_buff) {
            read_line_mode = true;
        } else if (io_buf->base == tty_output_buff) {
            unreachable();
        }
        break;
    case IOCTL_READ_MODE_ECHO: /* put readed symbol to stdout */
        if (io_buf->base == tty_input_buff) {
            is_echo = true;
        } else if (io_buf->base == tty_output_buff) {
            unreachable();
        }
        break;
    default:
        unreachable();
    }
}

/*
 * Write single character to tty
 */
static void tty_write_ch(struct io_buf_t* io_buf, char ch)
{
    if ((size_t)tty_output_buff_ptr - (size_t)tty_output_buff + 1 < VIDEO_SCREEN_SIZE) {
        if (ch != '\n') {
            /* regular character */
            *tty_output_buff_ptr++ = ch;
        } else {
            /* new line character */
            int line_pos = ((size_t)tty_output_buff_ptr - (size_t)tty_output_buff) % VIDEO_SCREEN_WIDTH;
            for (int j = 0; j < VIDEO_SCREEN_WIDTH - line_pos; ++j) {
                *tty_output_buff_ptr++ = ' ';
            }
        }
    } else {
        tty_output_buff_ptr = video_scroll(tty_output_buff, tty_output_buff_ptr);
        tty_write_ch(io_buf, ch);
    }

    io_buf->ptr = tty_output_buff_ptr;
}

/*
 * Read single character from tty
 */
static char tty_read_ch(struct io_buf_t* io_buf)
{
    io_buf->is_eof = (size_t)io_buf->ptr >= (size_t)tty_input_buff_ptr;
    io_buf->is_eol = false;

    if (!io_buf->is_eof && read_line_mode) {
        /* skip line */
        *((char *)tty_input_buff_ptr) = '\0';
        if (strchr(io_buf->ptr, '\n') == null) {
            io_buf->is_eof = true;
            return '\0';
        }
    }

    /* whether new character available */
    if (!io_buf->is_eof) {
        /* read character */
        char ch = *io_buf->ptr++;

        if (read_line_mode && ch == '\n') {
            io_buf->is_eof = true;
            io_buf->is_eol = true;
            tty_input_buff_ptr = tty_input_buff;
            io_buf->ptr = tty_input_buff_ptr;
            return '\0';
        }

        return ch;
    } else {
        /* correct pointer after backspace */
        io_buf->ptr = tty_input_buff_ptr;
    }

    return '\0';
}
