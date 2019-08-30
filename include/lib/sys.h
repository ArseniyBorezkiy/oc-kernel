#pragma once

#include <lib/stdtypes.h>

/*
 * Api
 */
extern void kill(u_int tid);
extern void exit(int errno) __attribute__ ((noreturn));
