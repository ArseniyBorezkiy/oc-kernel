#
# Memory management unit
#
.code32
.text
.globl asm_enable_paging, asm_disable_paging

/*
 * Enable paging
 * void asm_enable_paging(void *page_directory)
 */
asm_enable_paging:
    mov 4(%esp),%eax     # page_directory
    mov %eax,%cr3
    mov %cr0,%eax
    or $0x80000001,%eax  # set PE & PG bits
    mov %eax,%cr0
    ret

/*
 * Disable paging
 * void asm_disable_paging()
 */
asm_disable_paging:
    mov %eax,%cr3
    mov %cr0,%eax
    xor $0x80000000,%eax  # unset PG bit
    mov %eax,%cr0
    ret