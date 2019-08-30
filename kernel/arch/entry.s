#
# Kernel start code
#
.code32
.text
    # multiboot spec
    .align 4
multiboot:
    .long 0x1BADB002           # magic
    .long 0x00                 # flags
    .long -(0x1BADB002 + 0x00) # checksum. m+f+c should be zero

.global start
.extern kernel_start # this is defined in the c file

start:
    cli
    movl $stack,%esp
    push %esp
    push %ebx /* address of struct multiboot_t */
    call kernel_start /* should never return */
    hlt

#
# Kernel stack
#
.bss
.fill 8192,1 # 8Kb
stack:
