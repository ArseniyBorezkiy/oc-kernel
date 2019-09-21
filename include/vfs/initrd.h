#pragma once

#include <multiboot.h>
#include <lib/stdtypes.h>

#define INITRD_MAX_FILES 1
#define INITRD_MAGIC 0xBF

extern struct initrd_node_t {
    unsigned char magic; /* magic number */
    char name[8]; /* file name */
    unsigned int offset; /* file base */
    unsigned int length; /* file length */
};

extern struct initrd_fs_t {
    int count; /* files count */
    struct initrd_node_t node[INITRD_MAX_FILES]; /* files headers */
};

/*
 * Api
 */
extern void initrd_autorun();
extern void initrd_exec(char* name);

/*
 * Data
 */
extern size_t initrd_start;
extern size_t initrd_end;
