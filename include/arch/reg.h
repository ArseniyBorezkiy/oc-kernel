#pragma once

#include <lib/stdtypes.h>

/*
 * Api
 */
extern u16 asm_get_cs();
extern u16 asm_get_ds();
extern u16 asm_get_ss();
extern u32 asm_get_esp();
extern u32 asm_get_eflags();
extern u32 asm_get_cr0();
extern u32 asm_get_cr3();
extern void asm_switch_context(u32 esp);
