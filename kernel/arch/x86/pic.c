#include <arch/arch.h>
#include <lib/stdtypes.h>

/*
 * Api - Init programmable interrupt controller
 */
extern void pic_init()
{
    asm_write_port(PIC1_CMD_PORT, PIC_IQW_CMD); /* init PIC1 */
    asm_write_port(PIC2_CMD_PORT, PIC_IQW_CMD); /* init PIC2 */

    asm_write_port(PIC1_DATA_PORT, 0x20); /* IQW2 offset */
    asm_write_port(PIC2_DATA_PORT, 0x28); /* IQW2 offset */

    asm_write_port(PIC1_DATA_PORT, 0x00); /* IQW3 no cascade */
    asm_write_port(PIC2_DATA_PORT, 0x00); /* IQW3 no cascade */

    asm_write_port(PIC1_DATA_PORT, 0x01); /* IQW4 no cascade */
    asm_write_port(PIC2_DATA_PORT, 0x01); /* IQW4 no cascade */

    asm_write_port(PIC1_DATA_PORT, 0xff); /* disable irq PIC1 */
    asm_write_port(PIC2_DATA_PORT, 0xff); /* disable irq PIC2 */
}

/*
 * Api - Enable programmable interrupt controller
 */
extern void pic_enable()
{
    asm_write_port(PIC1_DATA_PORT, 0xFC); /* Enable IRQ0, IRQ1 */
}
