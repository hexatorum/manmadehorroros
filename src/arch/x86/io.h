#ifndef X86_IO_H
#define X86_IO_H

#include <stdint.h>

static inline uint8_t in(uint16_t port)
{
    uint8_t result;

    __asm__ volatile(
        "inb %w1, %b0"
        : "=a"(result)
        : "Nd"(port)
        : "memory"
    );

    return result;
}

static inline void out(uint16_t port, uint8_t value)
{
    __asm__ volatile("outb %b0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

static inline void wait()
{
    out(0x80, 0);
}

#endif