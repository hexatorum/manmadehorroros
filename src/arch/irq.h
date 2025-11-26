#ifndef PIC_H
#define PIC_H

#include <stdint.h>

void irq_init(uint8_t offset);
void irq_eoi(uint8_t irq);

#endif