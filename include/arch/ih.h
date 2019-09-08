#pragma once

#include <lib/stddef.h>
#include <lib/stdint.h>
#include <lib/stdbool.h>

/*
 * Api - IDT
 */
extern void ih_double_fault();
extern void ih_general_protect();
extern void ih_page_fault();
extern void ih_alignment_check();
extern void asm_ih_double_fault();
extern void asm_ih_general_protect();
extern void asm_ih_page_fault();
extern void asm_ih_alignment_check();

/*
 * Api - IRQ
 */
extern void ih_keyboard();
extern void asm_ih_keyboard();

