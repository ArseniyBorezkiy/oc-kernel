#pragma once

#include <lib/stdtypes.h>

#define IPC_MSG_DATA_BUFF_SIZE 80

struct message_t {
    u_short type; /* message type */
    u_int len; /* data length */
    u8 data[IPC_MSG_DATA_BUFF_SIZE]; /* message data */
};

/*
 * Api
 */

extern void ksend(u_short tid, struct message_t* msg);
extern void kreceive(u_short tid, struct message_t* msg);
