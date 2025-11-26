#include <kernel/serial.h>

#include <arch/io.h>

#define SERIAL_BASE 0x3F8

static bool serial_initialized = false;

bool serial_init()
{
    out(SERIAL_BASE + 1, 0x00);
    out(SERIAL_BASE + 3, 0x80);
    out(SERIAL_BASE, 0x03);
    out(SERIAL_BASE + 1, 0x00);
    out(SERIAL_BASE + 3, 0x03);
    out(SERIAL_BASE + 2, 0xC7);
    out(SERIAL_BASE + 4, 0x0B);
    out(SERIAL_BASE + 4, 0x1E);
    out(SERIAL_BASE, 0xAE);

    if (in(SERIAL_BASE) != 0xAE)
        return false;

    out(SERIAL_BASE + 4, 0x0F);

    serial_initialized = true;
    return true;
}

bool serial_is_initialized()
{
    return serial_initialized;
}

void serial_write(char value)
{
    while ((in(SERIAL_BASE + 5) & 0x20) == 0);
    
    out(SERIAL_BASE, value);
}

void serial_puts(const char *string)
{
    while (*string)
    {
        serial_write(*string++);
    }
}