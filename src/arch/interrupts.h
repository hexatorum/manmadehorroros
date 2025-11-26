#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    uint16_t base_low;
    uint16_t segment;
    uint8_t _unused;
    uint8_t attributes;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

typedef void (*interrupt_handler_t)();

void interrupts_enable();
void interupts_disable();

void interrupts_set(uint8_t interrupt, interrupt_handler_t handler);

void idt_init();

#endif