#pragma once

#include <types.h>

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

/*
 * Port operations
 */
extern u_char read_port(u_char port);
extern void write_port(u_char port, u_char data);
