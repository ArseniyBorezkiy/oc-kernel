#pragma once

#include <lib/stdtypes.h>

struct elf_header_ident_t {
    u32 ei_magic; /* elf signature */
    u8 ei_class; /* whether 32 or 64 bit */
    u8 ei_data; /* whether little-endian or big-endian encoding */
    u8 ei_version;
    u8 ei_osabi;
    u8 ei_abiversion;
    u8 ei_pad[7];
};

struct elf_program_header_t {
    u32 p_type; /* segment type */
    u32 p_offset; /* segment offset from file begin */
    u32 p_vaddr; /* target virtual address */
    u32 p_paddr; /* target physical address */
    u32 p_filesz; /* segment size in file */
    u32 p_memsz; /* segment size in memory */
    u32 p_flags; /* permissions and etc */
    u32 p_align; /* alignment */
} attribute(packed);

struct elf_header_t {
    struct elf_header_ident_t e_ident;
    u16 e_type;
    u16 e_machine;
    u32 e_version;
    u32 e_entry; /* virtual address of entry point */
    u32 e_phoff; /* program headers table offset */
    u32 e_shoff; /* program headers sections table offset */
    u32 e_flags;
    u16 e_ehsize; /* file header size */
    u16 e_phentsize; /* single header size */
    u16 e_phnum; /* headers count */
    u16 e_shentsize; /* section header size */
    u16 e_shnum; /* sections headers count */
    u16 e_shstrndx;
};

#define EI_MAGIC 0x464c457f
#define ELFCLASS32 1
#define ELFDATA2LSB 1
#define EV_CURRENT 1
#define ET_EXEC 2
#define EM_386 3
#define PT_LOAD 1

/*
 * Api
 */
extern void elf_exec(struct elf_header_t* header);
extern void elf_dump(struct elf_header_t* header);
