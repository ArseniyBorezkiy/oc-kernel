#pragma once

#include <types.h>

/*
 * Api
 */
extern unsigned int asm_get_cs();
extern unsigned int asm_get_ds();
extern unsigned int asm_get_ss();
extern unsigned int asm_get_sp();
extern unsigned int asm_get_flags();
extern void asm_switch_context(u_int sp);
