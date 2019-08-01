#pragma once

#include <types.h>

/*
 * i80386 general purpose registers
 */

struct gp_registers_t {
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 esp;
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;
};

/*
 * i80386 other purpose registers
 */
struct op_registers_t {
    u32 eip;
    u32 esp;
    u16 cs;
    u16 ds;
    u16 ss;
};

/*
 * i80386 flags
 */
struct flags_t {
    char cflag: 1; /* carry flag */
    char res1: 1; /* reserved */
    char pflag: 1; /* parity flag */
    char res2: 1; /* reserved */
    char aflag: 1; /* auxiliary carry flag */
    char res3: 1; /* reserved */
    char zflag: 1; /* zero flag */
    char sflag: 1; /* sign flag */
    char tflag: 1; /* trap flag */
    char iflag: 1; /* interrupt enable flag */
    char dflag: 1; /* direction flag */
    char oflag: 1; /* overflow flag */
    char iopl: 2; /* io privilege level */
    char nt: 1; /* nested task */
    char res4: 1; /* reserved */

    char rflag: 1; /* resume flag */
    char vmflag: 1; /* virtual mode flag */
    char acflag: 1; /* alignment check flag */
    char viflag: 1; /* virtual interrupt flag */
    char vipflag: 1; /* virtual interrupt pending flag */
    char idflag: 1; /* id flag */
    char res5: 10; /* reserved */
};
