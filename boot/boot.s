#
# boot sector
#
.set BOOT_SEGMENT,0x0000
.set KERNEL_SEGMENT,0xD000
.set KERNEL_OFFSET,0x0000

.text
.code16
.global _start
    jmp $BOOT_SEGMENT,$_start

#
# entry point from bios
# void _start()
#
_start:
call init_protected_mode
ret
    mov %cs,%ax
    mov %ax,%ds
    pushw $message_empty
    lcall $BOOT_SEGMENT,$printk
    #call read_kernel                     # read kernel
    #lcall $KERNEL_SEGMENT,$KERNEL_OFFSET # enter kernel code
    call boot_kernel
    pushw $message_loaded
    lcall $BOOT_SEGMENT,$printk
    jmp halt

#
# infinite loop
# halt()
#
halt:
    jmp halt

#
# print message
# __long void printk(char* message)
#
printk:
    pushw %bp
    movw %sp,%bp
    movw 6(%bp),%si
    xor %ax,%ax
    cld
    movb $0x0E,%ah
    movb $0x00,%bh
printk_loop:
    lodsb
    testb %al,%al
    jz printk_exit
    int $0x10
    jmp printk_loop
printk_exit:
    movw %bp,%sp
    popw %bp
    lret $2

#
# reading kernel
# void read_kernel()
#
read_kernel:
    pushw $message_reading
    lcall $BOOT_SEGMENT,$printk
    pushw $KERNEL_SEGMENT
    pushw $KERNEL_OFFSET
    call floppy_read
    jnc read_kernel_exit
    pushw $message_error
    lcall $BOOT_SEGMENT,$printk
read_kernel_exit:
    ret

#
# floppy read
# void floppy_read(word segment, word offset)
#
floppy_read:
    pushw %bp
    movw %sp,%bp
    movw 4(%bp),%bx
    movw 6(%bp),%ax
    movw %ax,%es
    xor %ax,%ax
    mov $0x02,%ah # read
    mov $1,%al # count
    mov $0,%ch # cylinder
    mov $2,%cl # sector
    mov $0,%dh # head
    mov $0,%dl # floppy drive
    int $0x13
    movw %bp,%sp
    popw %bp
    ret $4

#
# messages
#
message_empty: .asciz "\r\n"
message_reading: .asciz "[BOOT]: loading kernel from floppy\r\n"
message_error: .asciz "[BOOT]: an error occuried\r\n"
message_loaded: .asciz "[BOOT]: kernel loaded\r\n"
message_protected: .asciz "[BOOT]: launching protected mode\r\n"

#
# kernel boot sector
# __long void boot_kernel()
#
.code16
boot_kernel:
    pushw $message_protected
    lcall $BOOT_SEGMENT,$printk
    call init_protected_mode
    ret

#
# section data
# for kernel boot sector
#
message_protected_mode: .asciz "[BOOT]: protected mode enabled\r\n"

.align 8
# descriptors table
# descriptor structure
# 0  1  2  3    4    5    6    7  | 8    9  10 11   12   13 14   15 | limit limit
# 16 17 18 19   20   21   22   23 | 24   25 26 27   28   29 30   31 | base base
# 32 33 34 35   36   37   38   39 | 40 | 41 42 43 | 44 | 45 46 | 47 | base A type S DPL P
# 48 49 50 51 | 52 | 53 | 54 | 55 | 56   57 58 59   60   61 62   63 | limit AVL 0 DB G base
gdt:
    # zero descriptor
    .byte  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    # code descriptor
    .byte  0xFF, 0xFF # limit limit
    .byte  0x00, 0x00 # base base
    .byte  0x00, 0b10011000, # base present ring-0 system code
    .byte  0b11001111, 0x00 # limit 32bit 4Gb base
    # data descriptor
    .byte  0xFF, 0xFF # limit limit
    .byte  0x00, 0x00 # base base
    .byte  0x00, 0b10010100, # base present ring-0 system data
    .byte  0b11001111, 0x00 # limit 32bit 4Gb base
    # stack descriptor
    .byte  0xFF, 0xFF # limit limit
    .byte  0x00, 0x00 # base base
    .byte  0x00, 0b10010110, # base present ring-0 system stack
    .byte  0b11001111, 0x00 # limit 32bit 4Gb base
gdt_end:
    .set gdt_size,gdt_end-gdt
    .set SELECTOR_CODE,0b00001000 # gdt 0-ring 1 descriptor
    .set SELECTOR_DATA,0b00010000 # gdt 0-ring 2 descriptor
    .set SELECTOR_STACK,0b00011000 # gdt 0-ring 3 descriptor

gdtr_register:
    .word gdt_size-1 # limit
    .long gdt        # address

#
# switch to protected mode
# void init_protected_mode()
#
init_protected_mode:
    # open A20
    in $0x92,%al
    or $2,%al
    out %al,$0x92
    # load descriptor table
    lgdt gdtr_register
    # disable interrupts
    cli
    in $0x70,%al
    or $0x80,%al
    out %al,$0x70
    # enable protected mode
    mov %cr0,%eax
    or $1,%al
    mov %eax,%cr0
    # go to protected mode
    ljmpl $SELECTOR_CODE,$protected_mode_entry
    ret
    .asciz "LONG JUMP"

#
# protected mode entry point
# __long void protected_mode_entry()
#
.code32
protected_mode_entry:
    jmp .
    mov %cs,%ax
    mov %ax,%ds
    mov %ax,%es

    mov $0xB8000,%edi
    mov message_protected_mode,%esi
    cld
protected_mode_entry_loop:
    lodsb
    test %al,%al
    jz protected_mode_entry_halt
    stosb
    mov $7,%al
    stosb
    jmp protected_mode_entry_loop
protected_mode_entry_halt:
    jmp protected_mode_entry_halt

#
# boot sector end
#
boot_sector_signature:
.org 0x1FE,0
    .word 0xAA55
