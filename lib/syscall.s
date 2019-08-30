#
# System call to kernel
#
.code32
.text
.globl asm_syscall

/*
 * Call kernel
 * void asm_syscall(unsigned int function, ...)
 */
asm_syscall:
    push %ebx
    push %ebp
    mov %esp,%ebp
    mov %ebp,%ebx
    add $8,%ebx # skip registers
    add $4,%ebx # skip return address
    push %ebx # &function
    int $0x80
    mov %ebp,%esp
    pop %ebp
    pop %ebx
    ret
