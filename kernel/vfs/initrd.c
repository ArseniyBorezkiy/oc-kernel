#include <lib/assert.h>
#include <lib/stdio.h>
#include <lib/string.h>
#include <messages.h>
#include <mm/mm.h>
#include <vfs/elf.h>
#include <vfs/initrd.h>

static struct initrd_node_t* initrd_get_node(char* name, struct initrd_fs_t* fs);

/*
 * Data 
 */
size_t initrd_start = 0;
size_t initrd_end = 0;
static char* shell_elf = "sh.elf";

/*
 * Api - run initial ram disk elf file
 */
extern void initrd_autorun()
{
    printf(MSG_KERNEL_INITRD_AREA, initrd_start, initrd_end);

    /* autorun files */
    initrd_exec(shell_elf);
}

/*
 * Api - load elf
 */
extern void initrd_exec(char* name)
{
    struct initrd_fs_t* fs;
    struct initrd_node_t* node;
    struct elf_header_t* elf;

    /* find file */
    fs = (struct initrd_fs_t*)initrd_start;
    assert(fs->count > 0);
    node = initrd_get_node(name, fs);
    assert(node != null);

    /* exec elf */
    elf = (struct elf_header_t*)(node->offset + initrd_start);
    elf_exec(elf);
}

/*
 * Get initrd file node
 */
static struct initrd_node_t* initrd_get_node(char* name, struct initrd_fs_t* fs)
{
    struct initrd_node_t* node;

    for (int i = 0; i < fs->count; ++i) {
        node = &fs->node[i];
        assert(node->length > 0);
        assert(node->offset > 0);
        assert(node->magic == INITRD_MAGIC);

        if (!strcmp(name, node->name)) {
            return node;
        }
    }

    return null;
}
