#pragma once

#include <lib/stdtypes.h>

/*
 * i80386 general purpose registers
 */

struct gp_registers_t {
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 esp; /* current stack */
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;
} attribute(packed);

/*
 * i80386 other purpose registers
 */
struct op_registers_t {
    u32 eip;
    u32 cr3; /* page directory */
    u32 k_esp; /* kernel stack */
    u32 u_esp; /* user stack */
    u16 cs;
    u16 ds;
    u16 ss;
};

/*
 * i80386 flags
 */
struct flags_t {
    u8 cflag : 1; /* carry flag */
    u8 res1 : 1; /* reserved */
    u8 pflag : 1; /* parity flag */
    u8 res2 : 1; /* reserved */
    u8 aflag : 1; /* auxiliary carry flag */
    u8 res3 : 1; /* reserved */
    u8 zflag : 1; /* zero flag */
    u8 sflag : 1; /* sign flag */
    u8 tflag : 1; /* trap flag */
    u8 iflag : 1; /* interrupt enable flag */
    u8 dflag : 1; /* direction flag */
    u8 oflag : 1; /* overflow flag */
    u8 iopl : 2; /* io privilege level */
    u8 nt : 1; /* nested task */
    u8 res4 : 1; /* reserved */

    u8 rflag : 1; /* resume flag */
    u8 vmflag : 1; /* virtual mode flag */
    u8 acflag : 1; /* alignment check flag */
    u8 viflag : 1; /* virtual interrupt flag */
    u8 vipflag : 1; /* virtual interrupt pending flag */
    u8 idflag : 1; /* id flag */
    u16 res5 : 10; /* reserved */
} attribute(packed);

/*
 * Api
 */
extern u16 asm_get_cs();
extern u16 asm_get_ds();
extern u16 asm_get_ss();
extern u32 asm_get_esp();
extern u32 asm_get_eflags();
extern u32 asm_get_cr0();
extern u32 asm_get_cr2();
extern u32 asm_get_cr3();
extern void asm_lock();
extern void asm_unlock();
extern void asm_hlt();
