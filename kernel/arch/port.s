#
# Port operations
#
.code32
.text
.globl read_port, write_port

/*
 * Read data from port
 * unsigned char read_port(unsigned char port)
 */
read_port:
    mov 4(%esp), %edx
    in %dx, %al
    ret

/*
 * Send data to port
 * void write_port(unsigned char port, unsigned_char data)
 */
write_port:
    mov   4(%esp), %edx
    mov   8(%esp), %al
    out   %al, %dx
    ret
