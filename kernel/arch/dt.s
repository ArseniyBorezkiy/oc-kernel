#
# Global descriptor table helpers
#
.code32
.text
.globl asm_gdt_load, asm_idt_load, asm_tss_load
.globl asm_load_data_kselectors, asm_load_data_uselectors
.globl asm_switch_kcontext, asm_switch_ucontext

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

/*
 * Load interrupt table
 * void asm_idt_load(unsigned long *addr)
 */
asm_idt_load:
    push %edx
    mov 8(%esp), %edx
    lidt (%edx)
    pop %edx
    ret

/*
 * Load task state segment
 * void asm_tss_load(u_int index)
 */
asm_tss_load:
    mov 4(%esp),%eax # eax = index
    ltr %ax
    ret

/*
 * Load kernel selectors
 */
 asm_load_data_kselectors:
    push %eax
    mov $0x10,%eax
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
    pop %eax
    ret

/*
 * Load user selectors
 */
 asm_load_data_uselectors:
    push %eax
    mov $0x2b,%eax
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
    pop %eax
    ret

/*
 * Switch context (to kernel ring)
 * void asm_switch_kcontext(u32 esp, u32 cr3)
 */
asm_switch_kcontext:
    mov 4(%esp),%ebp # ebp = esp
    mov 8(%esp),%eax # eax = cr3
    mov %cr0,%ebx    # ebx = cr0
    xor $0x80000000,%ebx  # unset PG bit
    mov %ebx,%cr0
    mov %eax,%cr3
    or $0x80000001,%ebx  # set PE & PG bits
    mov %ebx,%cr0
    mov %ebp,%esp
    popal
    sti
    iretl

/*
 * Switch context (from kernel to user ring)
 * void asm_switch_ucontext(u32 esp, u32 cr3)
 */
asm_switch_ucontext:
    mov 4(%esp),%ebp # ebp = esp
    mov 8(%esp),%eax # eax = cr3
    mov %cr0,%ebx    # ebx = cr0
    xor $0x80000000,%ebx  # unset PG bit
    mov %ebx,%cr0
    mov %eax,%cr3
    or $0x80000001,%ebx  # set PE & PG bits
    mov %ebx,%cr0
    mov %ebp,%esp
    popal
    call asm_load_data_uselectors
    sti
    iretl
