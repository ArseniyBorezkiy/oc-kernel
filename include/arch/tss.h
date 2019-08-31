#pragma once

#include <lib/stdtypes.h>

/*
 * Task state segment
 */
struct TSS_entry_t {
    u16 link;
    u16 reserved0;
    u32 esp0;
    u16 ss0;
    u16 reserved1;
    u32 esp1;
    u16 ss1;
    u16 reserved2;
    u32 esp2;
    u16 ss2;
    u16 reserved3;
    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u16 es;
    u16 reserved4;
    u16 cs;
    u16 reserved5;
    u16 ss;
    u16 reserved6;
    u16 ds;
    u16 reserved7;
    u16 fs;
    u16 reserved8;
    u16 gs;
    u16 reserved9;
    u16 ldtr;
    u16 reserved10;
    u16 reserved11;
    u16 iopb;
};
