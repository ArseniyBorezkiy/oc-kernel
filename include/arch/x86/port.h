#pragma once

#include <lib/stdtypes.h>

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

/*
 * Port operations
 */
extern u_char asm_read_port(u_char port);
extern void asm_write_port(u_int port, u_char data);
