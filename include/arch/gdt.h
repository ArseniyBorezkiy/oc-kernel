#pragma once

#include <lib/stdtypes.h>

/*
 * Global descriptor table entry
 */
struct GDT_entry {
    u16 limit_low: 16;
    u16 base_low: 16;
    u8 base_middle: 8;
    u8 type: 4;
    u8 s: 1;
    u8 dpl: 2;
    u8 p: 1;
    u8 limit_high: 4;
    u8 a: 1;
    u8 reserved: 1;
    u8 db: 1;
    u8 g: 1;
    u8 base_high: 8;
};
