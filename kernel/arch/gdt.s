#
# Global descriptor table helpers
#
.code32
.text
.globl asm_gdt_load

/*
 * Load global descriptor table
 * void asm_gdt_load(void *gdt_ptr)
 */
asm_gdt_load:
    mov 4(%esp),%eax # eax = gdt_ptr
    lgdt (%eax)
    mov $0x10,%eax
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
    mov %ax,%ss
    jmp $0x08,$asm_gdt_load_exit
asm_gdt_load_exit:
    ret
