#pragma once

#include <lib/stdtypes.h>
#include <lib/data/clist.h>

/*
 * Api
 */
extern bool kill(u_int tid);
extern void exit(int errno) __attribute__ ((noreturn));
extern struct clist_definition_t* ps();
extern struct clist_definition_t* devs();
extern bool exec(char *name);
