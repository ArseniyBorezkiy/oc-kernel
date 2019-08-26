#include <arch/reg.h>
#include <vfs/elf.h>
#include <mm/mm.h>
#include <sched/task.h>
#include <lib/assert.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <messages.h>

static u_short next_tid = TID_USER; /* tid allocator */

/*
 * Api - load elf file to memory
 */
extern void elf_load(struct elf_header_t *header) {
  assert(header->e_ident.ei_magic == EI_MAGIC);

  printf(MSG_KERNEL_ELF_LOADING, header->e_phnum);
  elf_dump(header);

  size_t elf_base = (size_t)header;
  size_t entry_point = header->e_entry;

  // load sections in memory
  for (int i = 0; i < header->e_phnum; ++i) {
    struct elf_program_header_t *p_header = (void *)(header->e_phoff + elf_base + i * header->e_phentsize);
    // allocate pages
    u_int pages = (p_header->p_memsz / MM_PAGE_SIZE) + 1;
    assert(pages == 1); /* our elfs they are all small */
    void *page = mm_alloc_page();
    void *section = (void *)(elf_base + p_header->p_offset);
    memcpy(page, section, p_header->p_memsz);
    // set page directory
  }

  // create task
  u_short tid = next_tid++;
  assert(task_create(tid, (void *)entry_point));

  // run task
  struct task_t *task;
  task = task_get_by_id(tid);
  task->status = TASK_RUNNING;
  strncpy(task->name, "elf", sizeof(task->name));
}

/*
 * Api - elf header dump
 */
extern void elf_dump(struct elf_header_t *header)
{
  size_t elf_base = (size_t)header;

  printf("-- elf header dump: %X\n", elf_base);
  printf("  entry point: %X\n", header->e_entry);
  printf("  headers count: %X\n", header->e_phnum);
  printf("  headers begin: %X (%X)\n", header->e_phoff, header->e_phoff + elf_base);

  for (int i = 0; i < header->e_phnum; ++i)
  {
    struct elf_program_header_t *p_header = (void *)(header->e_phoff + elf_base + i * header->e_phentsize);

    if (p_header->p_filesz == 0) {
      continue;
    }

    printf("  + program header dump: %X\n", p_header);
    printf("    segment type: %X\n", p_header->p_type);
    printf("    target virtual address: %X\n", p_header->p_vaddr);
    printf("    segment size in memory: %X\n", p_header->p_memsz);
    printf("    segment offset from file begin: %X\n", p_header->p_offset);
    printf("    segment size in file: %X\n", p_header->p_filesz);
    printf("    alignment: %X\n", p_header->p_align);
    
    assert(header->e_phentsize == sizeof(struct elf_program_header_t));
  }
}
