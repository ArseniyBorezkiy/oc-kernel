#
# Port operations
#
.code32
.text
.globl asm_read_port, asm_write_port

/*
 * Read data from port
 * unsigned char asm_read_port(unsigned char port)
 */
asm_read_port:
    mov 4(%esp), %edx
    in %dx, %al
    ret

/*
 * Send data to port
 * void asm_write_port(unsigned char port, unsigned_char data)
 */
asm_write_port:
    mov   4(%esp), %edx
    mov   8(%esp), %al
    out   %al, %dx
    ret
