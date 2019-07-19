#pragma once

#define HIGH_WORD(addr) ((addr & 0xffff0000) >> 16)
#define LOW_WORD(addr) ((addr & 0xffff))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define true 1
#define false 0
