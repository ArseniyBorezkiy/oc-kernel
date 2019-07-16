#pragma once

#define HIGH_WORD(addr) ((addr & 0xffff0000) >> 16)
#define LOW_WORD(addr) ((addr & 0xffff))
