#
# Registers
#
.code32
.text
.globl asm_get_cs, asm_get_ds, asm_get_ss, asm_get_esp, asm_get_eflags, asm_switch_context
.globl asm_get_cr0, asm_get_cr3

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

#
# Switch context
# void asm_switch_context(u32 esp, u32 cr3)
#
asm_switch_context:
  mov 4(%esp),%ebp # ebp = esp
  mov 8(%esp),%eax # eax = cr3
  mov %eax,%cr3
  mov %ebp,%esp
  popal
  sti
  iretl
