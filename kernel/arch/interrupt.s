#
# Interrupt descriptor table helpers
#
.code32
.text
.globl load_idt, keyboard_handler

/*
 * Load interrupt table
 * void load_idt(unsigned long *addr)
 */
load_idt:
	mov 4(%esp), %edx
	lidt (%edx)
	sti
	ret

/*
 * Handle IRQ1
 * void keyboard_handler()
 */
keyboard_handler:
	call keyboard_handler_main
	iretl
