#
# Interrupt descriptor table helpers
#
.code32
.text
.globl asm_idt_load, asm_ih_timer, asm_ih_keyboard

/*
 * Load interrupt table
 * void asm_idt_load(unsigned long *addr)
 */
asm_idt_load:
	mov 4(%esp), %edx
	lidt (%edx)
	sti
	ret

/*
 * Handle IRQ0
 * void asm_ih_timer(unsigned int)
 */
asm_ih_timer:
	call ih_timer
	iretl

/*
 * Handle IRQ1
 * void asm_ih_keyboard(unsigned int)
 */
asm_ih_keyboard:
	call ih_keyboard
	iretl
