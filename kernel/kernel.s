#
# Kernel start code
#
.code32
.text
        # multiboot spec
        .align 4
        .long 0x1BADB002           # magic
        .long 0x00                 # flags
        .long -(0x1BADB002 + 0x00) # checksum. m+f+c should be zero

.global start
.extern kmain # this is defined in the c file

start:
	cli
	mov stack,%esp
	call kmain
	hlt

#
# Kernel stack
#
.bss
.fill 8192,1 # 8Kb
stack:
