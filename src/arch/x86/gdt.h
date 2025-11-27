#ifndef X86_GDT_H
#define X86_GDT_H

#include <stdint.h>

typedef struct
{
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr_t;

void gdt_init();

#endif