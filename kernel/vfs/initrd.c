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
static char* shell_elf = "sh.elf";

/*
 * Api - run initial ram disk elf file
 */
extern void initrd_autorun(struct mod_addr_t* mods_addr, int mods_count)
{
    assert(mods_count > 0);

    u32 start = mods_addr[0].start;
    u32 end = mods_addr[0].end;

    printf(MSG_KERNEL_INITRD_AREA, start, end, mods_count);

    /* autorun files */
    initrd_exec(shell_elf, start);
}

/*
 * Api - load elf
 */
extern void initrd_exec(char* name, size_t base)
{
    struct initrd_fs_t* fs;
    struct initrd_node_t* node;
    struct elf_header_t* elf;

    /* find file */
    fs = (struct initrd_fs_t*)base;
    assert(fs->count > 0);
    node = initrd_get_node(name, fs);
    assert(node != null);

    /* exec elf */
    elf = (struct elf_header_t*)(node->offset + base);
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
