#
# Interrupt descriptor table helpers
#
.code32
.text
.globl asm_idt_load, asm_ih_timer, asm_ih_keyboard, asm_lock, asm_unlock, asm_hlt

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
 * Handle IRQ0
 * void asm_ih_timer(unsigned long *addr)
 */
asm_ih_timer:
    cli
    pushal
    mov %esp,%ebp
    mov %ebp,%ebx
    add %ebx,32 # ebx = &ret_addr
    push %ebx
	call ih_timer
	mov %ebp,%esp
	popal
	sti
	iretl

/*
 * Handle IRQ1
 * void asm_ih_keyboard(unsigned int)
 */
asm_ih_keyboard:
    pushal
	call ih_keyboard
	popal
	iretl

/*
 * Lock interrupts
 */
asm_lock:
    cli
    ret

/*
 * Unlock interrupts
 */
asm_unlock:
    sti
    ret

/*
 * Halt processor
 */
asm_hlt:
    hlt
    ret
