#
# Registers
#
.code32
.text
.globl asm_lock, asm_unlock, asm_hlt

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
