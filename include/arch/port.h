#pragma once

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

/*
 * Port operations
 */
unsigned char read_port(unsigned char port);
void write_port(unsigned char port, unsigned char data);
