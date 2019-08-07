#include <arch/reg.h>
#include <arch/idt.h>
#include <sched/task.h>
#include <tasks/tty.h>
#include <ipc/ipc.h>
#include <utils/kprint.h>
#include <utils/kassert.h>
#include <lib/time.h>
#include <lib/string.h>
#include <lib/stdtypes.h>
#include <messages.h>

static u_short tid_stdin = TID_TTY; /* target tid to send incomming chars */
static char *video_ptr; /* current position in video memory */

static void handle_getc(char ch);
static void handle_putc(char ch);
static void handle_puts(char *str);
static void handle_clear();
static void scroll(u_int n);

/*
 * Api - Teletype task
 */
extern void task_tty_main() {
    struct message_t msg;

    kprint(MSG_TASK_TTY_LAUNCHED, (size_t*)asm_get_esp());
    init_syslog(); /* disable early kprint */
    handle_clear();
    
    while (1) {
        kreceive(TID_TTY, &msg);
        
        /* decode message */
        switch (msg.type) {
            case TTY_MSG_TYPE_GETC: {
                /* key pressed */
                kassert(__FILE__, __LINE__, msg.len == 1);
                handle_getc(msg.data[0]);
                break;
            }
            case TTY_MSG_TYPE_PUTC: {
                /* put character to screen */
                kassert(__FILE__, __LINE__, msg.len == 1);
                handle_putc(msg.data[0]);
                break;
            }
            case TTY_MSG_TYPE_PUTS: {
                /* put string to screen */
                kassert(__FILE__, __LINE__, msg.data[msg.len - 1] == 0);
                handle_puts((char*)msg.data);
                break;
            }
            case TTY_MSG_TYPE_CLEAR: {
                /* clear screen */
                handle_clear();
                break;
            }
        }
    }
}

/*
 * Handle key pressed
 */
static void handle_getc(char ch) {
    struct message_t msg;

    /* send to stdin */
    msg.type = tid_stdin == TID_TTY ? TTY_MSG_TYPE_PUTC : TTY_MSG_TYPE_GETC;
    msg.len = 1;
    msg.data[0] = ch;
    ksend(tid_stdin, &msg);
}

/*
 * Handle put character to screen
 */
static void handle_putc(char ch) {
    if (ch != '\n') {
        /* usual character */
        size_t delta = (video_ptr - (char*)VIDEO_MEMORY_ADDR);
        if (delta + 2 >= SCREEN_SIZE) {
            scroll(1); /* scroll line up */
        }

        *video_ptr = ch;
        video_ptr += 2;
    } else {
        /* new line character */
        size_t delta = (video_ptr - (char*)VIDEO_MEMORY_ADDR);
        size_t offset = delta % SCREEN_WIDTH;

        if (delta + offset >= SCREEN_SIZE) {
            scroll(1); /* scroll line up */
        }

        video_ptr += (SCREEN_WIDTH - offset);
    }
}

/*
 * Handle put string to screen
 */
static void handle_puts(char *str) {
    while (*str != 0) {
        handle_putc(*str);
    }
}

/*
 * Clear screen
 */
static void handle_clear() {
    video_ptr = (char*)VIDEO_MEMORY_ADDR;
    memset(video_ptr, VIDEO_MEMORY_ATTR, SCREEN_SIZE);
}

/*
 * Scroll console up
 */
static void scroll(u_int n) {
    char *ptr = (char*)VIDEO_MEMORY_ADDR;
    for (int i = 1; i < SCREEN_HEIGHT * 2; ++i) {
        for (int j = 0; j < SCREEN_WIDTH * 2; ++j) {
            ptr[(i - 1) * SCREEN_WIDTH * 2 + j] = ptr[i * SCREEN_WIDTH * 2 + j];
        }
    }

    for (int j = 0; j < SCREEN_WIDTH; ++j) {
        ptr[(SCREEN_HEIGHT * 2 - 1) + j * 2] = ' ';
        ptr[(SCREEN_HEIGHT * 2 - 1) + j * 2 + 1] = VIDEO_MEMORY_ATTR;
    }

    video_ptr -= SCREEN_WIDTH * 2;
}
