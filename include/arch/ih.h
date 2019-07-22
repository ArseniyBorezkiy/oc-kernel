#pragma once

#include <types.h>

extern void ih_timer(size_t *ret_addr);
extern void ih_keyboard();
extern void asm_ih_keyboard();
extern void asm_ih_timer(size_t *ret_addr);
