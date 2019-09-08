#
# Interrupt descriptor table handlers
#
.code32
.text
.globl asm_ih_double_fault, asm_ih_general_protect
.globl asm_ih_alignment_check
.globl asm_ih_keyboard


/*
 * Handle error double fault
 */
 asm_ih_double_fault:
    cli
    pushal
    call asm_load_data_kselectors
    call ih_double_fault
    call asm_load_data_uselectors
    popal
    sti
    iretl

/*
 * General protect
 */
 asm_ih_general_protect:
    cli
    pushal
    call asm_load_data_kselectors
    call ih_general_protect
    call asm_load_data_uselectors
    popal
    sti
    iretl

/*
 * Alignment check
 */
 asm_ih_alignment_check:
    cli
    pushal
    call asm_load_data_kselectors
    call ih_alignment_check
    call asm_load_data_uselectors
    popal
    sti
    iretl

/*
 * Handle IRQ1
 * void asm_ih_keyboard(unsigned int)
 */
asm_ih_keyboard:
    pushal
    call asm_load_data_kselectors
    call ih_keyboard
    call asm_load_data_uselectors
    popal
    iretl
