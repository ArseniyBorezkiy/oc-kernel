#pragma once

/* types */
typedef unsigned long size_t;

#define HIGH_WORD(addr) ((addr & 0xffff0000) >> 16)
#define LOW_WORD(addr) ((addr & 0xffff))
#define LOW_BYTE(addr) ((addr & 0x00ff))
#define HIGH_BYTE(addr) ((addr & 0xff00) >> 8)
