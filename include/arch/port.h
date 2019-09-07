#pragma once

#include <lib/stdint.h>
#include <lib/stdbool.h>

/*
 * Port operations
 */
extern u_char asm_read_port(u_char port);
extern void asm_write_port(u_int port, u_char data);
