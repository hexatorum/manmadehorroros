#include <arch/irq.h>

#include <arch/io.h>

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_COMMAND 0xA0
#define PIC_SLAVE_DATA 0xA1

void irq_init(uint8_t offset)
{
    out(PIC_MASTER_COMMAND, 0x11);
    wait();
    out(PIC_SLAVE_COMMAND, 0x11);
    wait();

    out(PIC_MASTER_DATA, offset);
    wait();
    out(PIC_SLAVE_DATA, offset + 8);
    wait();

    out(PIC_MASTER_DATA, 4);
    wait();
    out(PIC_SLAVE_DATA, 2);
    wait();

    out(PIC_MASTER_DATA, 0x01);
    wait();
    out(PIC_SLAVE_DATA, 0x01);
    wait();

    out(PIC_MASTER_DATA, 0xFD);
	out(PIC_SLAVE_DATA, 0xFF);
}

void irq_eoi(uint8_t irq)
{
    if (irq >= 8)
        out(PIC_SLAVE_COMMAND, 0x20);

    out(PIC_MASTER_COMMAND, 0x20);
}