#
# Registers
#
.code32
.text
.globl asm_get_cs, asm_get_ds, asm_get_ss, asm_get_esp, asm_get_eflags
.globl asm_get_cr0, asm_get_cr2, asm_get_cr3
.globl asm_lock, asm_unlock, asm_hlt

#
# Get code selector
# u16 asm_get_cs()
#
asm_get_cs:
    xor %eax,%eax
    movw %cs,%ax
    ret

#
# Get data selector
# u16 asm_get_ds()
#
asm_get_ds:
    xor %eax,%eax
    movw %ds,%ax
    ret

#
# Get stack selector
# u16 asm_get_ss()
#
asm_get_ss:
    xor %eax,%eax
    movw %ss,%ax
    ret

#
# Get stack pointer
# u32 asm_get_esp()
#
asm_get_esp:
    mov %esp,%eax
    ret

#
# Get control register
# u32 asm_get_cr0()
#
asm_get_cr0:
    mov %cr0,%eax
    ret

#
# Get page fault linear address register
# u32 asm_get_cr2()
#
asm_get_cr2:
    mov %cr2,%eax
    ret
  
#
# Get page directory register
# u32 asm_get_cr3()
#
asm_get_cr3:
    mov %cr3,%eax
    ret

#
# Get flags register
# u32 asm_get_eflags()
#
asm_get_eflags:
    pushf
    popl %eax
    ret

/*
 * Lock interrupts
 * void asm_lock();
 */
asm_lock:
    cli
    ret

/*
 * Unlock interrupts
 * void asm_unlock();
 */
asm_unlock:
    sti
    ret

/*
 * Halt processor
 * void asm_hlt();
 */
asm_hlt:
    hlt
    ret
