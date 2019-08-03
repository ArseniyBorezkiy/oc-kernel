#
# Interrupt descriptor table helpers
#
.code32
.text
.globl asm_idt_load, asm_lock, asm_unlock, asm_hlt
.globl asm_ih_zero, asm_ih_opcode, asm_ih_double_fault, asm_ih_general_protect
.globl asm_ih_page_fault, asm_ih_alignment_check
.globl asm_ih_timer, asm_ih_keyboard

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
 * Handle error division by zero
 */
 asm_ih_zero:
  cli
  pushal
  call ih_zero
  popal
  sti
  iretl

/*
 * Handle error invalid opcode
 */
 asm_ih_opcode:
  cli
  pushal
  call ih_opcode
  popal
  sti
  iretl

/*
 * Handle error double fault
 */
 asm_ih_double_fault:
  cli
  pushal
  call ih_double_fault
  popal
  sti
  iretl

/*
 * General protect
 */
 asm_ih_general_protect:
  cli
  pushal
  call ih_general_protect
  popal
  sti
  iretl

/*
 * Page fault
 */
 asm_ih_page_fault:
  cli
  pushal
  call ih_page_fault
  popal
  sti
  iretl

/*
 * Alignment check
 */
 asm_ih_alignment_check:
  cli
  pushal
  call ih_alignment_check
  popal
  sti
  iretl

/*
 * Handle IRQ0
 * void asm_ih_timer(unsigned long *addr)
 */
asm_ih_timer:
  cli
  pushal
  mov %esp,%ebp
  mov %ebp,%ebx
  pushl %ebx # &reg
  add $32,%ebx
  pushl %ebx # &ret addr
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
