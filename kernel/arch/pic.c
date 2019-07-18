#include <arch/pic.h>
#include <arch/port.h>

/*
 * Init programmable interrupt controller
 */
void pic_init(void) {
    write_port(PIC1_CMD_PORT , PIC_IQW_CMD); /* init PIC1 */
    write_port(PIC2_CMD_PORT , PIC_IQW_CMD); /* init PIC2 */

    write_port(PIC1_DATA_PORT , 0x20); /* IQW2 offset */
    write_port(PIC2_DATA_PORT , 0x28); /* IQW2 offset */

    write_port(PIC1_DATA_PORT , 0x00); /* IQW3 no cascade */
    write_port(PIC2_DATA_PORT , 0x00); /* IQW3 no cascade */

    write_port(PIC1_DATA_PORT , 0x01); /* IQW4 no cascade */
    write_port(PIC2_DATA_PORT , 0x01); /* IQW4 no cascade */

    write_port(PIC1_DATA_PORT , 0xff); /* disable irq PIC1 */
    write_port(PIC2_DATA_PORT , 0xff); /* disable irq PIC2 */
}

/*
 * Enable programmable interrupt controller
 */
void pic_enable(void)
{
    write_port(PIC1_DATA_PORT, 0xFC); /* Enable IRQ0, IRQ1 */
}
