#pragma once

#define PIC1_CMD_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_CMD_PORT 0xA0
#define PIC2_DATA_PORT 0xA1
#define PIC_IQW_CMD 0x11

/*
 * Port operations
 */
unsigned char read_port(unsigned char port);
void write_port(unsigned char port, unsigned char data);
