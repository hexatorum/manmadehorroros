#include <arch/x86/gdt.h>

#include <stddef.h>

gdt_entry_t gdt[3] = {
    // null descriptor
    {},
    // code descriptor
    {
        .limit = 0xFFFF,
        .base_low = 0,
        .base_middle = 0,
        .access = 0b10011010,
        .granularity = 0b11001111,
        .base_high = 0
    },
    // data descriptor
    {
        .limit = 0xFFFF,
        .base_low = 0,
        .base_middle = 0,
        .access = 0b10010010,
        .granularity = 0b11001111,
        .base_high = 0
    }
};
gdtr_t gdtr = {};

void gdt_init()
{
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (uint32_t)&gdt;

    __asm__ volatile(
        "lgdt %0"
        :
        : "m"(gdtr)
    );

    __asm__ volatile(
        "ljmp $0x08, $.exit\n"
        ".exit:\n"
    );

    __asm__ volatile(
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        :
        :
        : "eax"
    );
}