#include <arch/arch.h>
#include <kernel.h>
#include <lib/assert.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/string.h>

extern void asm_enable_paging(void* page_directory);
extern void asm_disable_paging();

static struct page_directory_entry_t kpage_directory attribute(aligned(4096));
static struct page_table_entry_t kpage_table[MMU_PAGE_TABLE_ENTRIES_COUNT] attribute(aligned(4096));

/*
 * Api - init kernel page directory
 * Here assumed each entry addresses 4Kb
 */
extern void mmu_init()
{
    memset(&kpage_directory, 0, sizeof(struct page_directory_entry_t));

    /* set kernel page directory */
    kpage_directory.zero = 1;
    kpage_directory.accessed = 0;
    kpage_directory.available = 0;
    kpage_directory.cache_disabled = 0;
    kpage_directory.ignored = 0;
    kpage_directory.page_size = 0; /* 4KB */
    kpage_directory.present = 1; /* kernel pages always in memory */
    kpage_directory.read_write = 1; /* read & write */
    kpage_directory.user_supervisor = 1; /* kernel mode pages */
    kpage_directory.write_through = 1;
    kpage_directory.page_table_addr = (size_t)kpage_table >> 12;

    /* set kernel table */
    for (int i = 0; i < MMU_PAGE_TABLE_ENTRIES_COUNT; ++i) {
        kpage_table[i].zero = 0;
        kpage_table[i].accessed = 0;
        kpage_table[i].available = 0;
        kpage_table[i].cache_disabled = 0;
        kpage_table[i].dirty = 0;
        kpage_table[i].global = 1;
        kpage_table[i].present = 1; /* kernel pages always in memory */
        kpage_table[i].read_write = 1; /* read & write */
        kpage_table[i].user_supervisor = 1; /* kernel mode pages */
        kpage_table[i].write_through = 1;
        kpage_table[i].page_phys_addr = (i * 4096) >> 12; /* assume 4Kb pages */
    }
}

/*
 * Api - Enable paging
 */
extern void mmu_enable()
{
    asm_enable_paging(&kpage_directory);
}

/*
 * Api - Get kernel page directory
 */
extern struct page_directory_entry_t* mmu_get_kdirectory()
{
    return &kpage_directory;
}

/*
 * Api - Get kernel page table
 */
extern struct page_table_entry_t* mmu_get_ktable()
{
    return kpage_table;
}

/*
 * Api - Set active page directory
 */
extern void mmu_set_active_page_directory(struct page_directory_entry_t* upage_dir) {
    asm_disable_paging();
    asm_enable_paging(upage_dir);
}

/*
 * Api - Create user page directory
 */
extern struct page_directory_entry_t* mmu_create_user_page_directory(struct page_table_entry_t* page_table)
{
    struct page_directory_entry_t* upage_dir;
    upage_dir = malloc_a(sizeof(struct page_directory_entry_t), 4096);

    upage_dir->zero = 1;
    upage_dir->accessed = 0;
    upage_dir->available = 0;
    upage_dir->cache_disabled = 0;
    upage_dir->ignored = 0;
    upage_dir->page_size = 0; /* 4KB */
    upage_dir->present = 1;
    upage_dir->read_write = 1; /* read & write */
    upage_dir->user_supervisor = 0; /* user mode pages */
    upage_dir->write_through = 1;
    upage_dir->page_table_addr = (size_t)page_table >> 12; /* assume 4Kb pages */

    return upage_dir;
}

/*
 * Api - Create user page table
 */
extern struct page_table_entry_t* mmu_create_user_page_table()
{
    struct page_table_entry_t* upage_table;
    upage_table = malloc_a(sizeof(struct page_table_entry_t) * MMU_PAGE_TABLE_ENTRIES_COUNT, 4096);

    /* share kernel pages */
    memcpy(upage_table, kpage_table, sizeof(struct page_table_entry_t) * MMU_KERNEL_PAGES_COUNT);
    /* fill user pages */
    for (int i = MMU_KERNEL_PAGES_COUNT; i < MMU_PAGE_TABLE_ENTRIES_COUNT; ++i) {
        struct page_table_entry_t* current;
        current = upage_table + i;

        current->zero = 0;
        current->accessed = 0;
        current->available = 0;
        current->cache_disabled = 0;
        current->dirty = 0;
        current->global = 1;
        current->present = 0; /* not present as so as there is no user pages yet */
        current->read_write = 1; /* read & write */
        current->user_supervisor = 0; /* user mode page */
        current->write_through = 1;
        current->page_phys_addr = 0; /* page is not present */
    }

    return upage_table;
}

/*
 * Api - Occupy user page
 */
extern bool mmu_occupy_user_page(struct page_table_entry_t* upage_table, void* phys_addr)
{
    for (int i = MMU_KERNEL_PAGES_COUNT; i < MMU_PAGE_TABLE_ENTRIES_COUNT; ++i) {
        struct page_table_entry_t* current;
        current = upage_table + i;

        if (current->present) {
            /* page is buzy */
            continue;
        }

        current->zero = 0;
        current->accessed = 0;
        current->available = 0;
        current->cache_disabled = 0;
        current->dirty = 0;
        current->global = 1;
        current->present = 1;
        current->read_write = 1; /* read & write */
        current->user_supervisor = 0; /* user mode page */
        current->write_through = 1;
        current->page_phys_addr = (size_t)phys_addr >> 12; /* assume 4Kb pages */

        return true;
    }

    return false;
}

/*
 * Api - Free resources
 */
extern void mmu_destroy_user_page_directory(struct page_directory_entry_t* upage_dir, struct page_table_entry_t* upage_table)
{
    if (upage_dir != &kpage_directory) {
        free_a(upage_dir);
    }
    if (upage_table != kpage_table) {
        free_a(upage_table);
    }
}

/*
 * Api - Dump kernel page directory
 */
extern void mmu_kdump(u_int table_index)
{
    printf(" -- Kernel page directory dump (dir=%X (tables addr %X), tables addr=%X)\n", &kpage_directory, kpage_directory.page_table_addr, &kpage_table);

    assert(kpage_directory.page_table_addr == (size_t)&kpage_table >> 12);

    printf(" table %X\n", &kpage_table[table_index]);
    for (int i = table_index; i < table_index + 8; ++i) {
        printf("   phys=%X a=%X g=%X p=%X\n",
            kpage_table[i].page_phys_addr,
            kpage_table[i].available,
            kpage_table[i].global,
            kpage_table[i].present);
    }
}
