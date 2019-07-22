#
# Registers
#
.code32
.text
.globl asm_get_cs, asm_get_ds, asm_get_ss, asm_get_flags

#
# Get code selector
# unsinged short asm_get_cs()
#
asm_get_cs:
    xor %eax,%eax
    movw %cs,%ax
    ret

#
# Get data selector
# unsigned short asm_get_ds()
#
asm_get_ds:
    xor %eax,%eax
    movw %ds,%ax
    ret

#
# Get stack selector
# unsigned short asm_get_ss()
#
asm_get_ss:
    xor %eax,%eax
    movw %ss,%ax
    ret

#
# Get flags register
# unsigned int asm_get_flags()
#
asm_get_flags:
    pushf
    popl %eax
    ret
