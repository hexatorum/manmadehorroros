#include <kernel/ps2.h>

#include <arch/io.h>
#include <lib/bits.h>

#define PS2_DATA 0x60
#define PS2_STATUS 0x64
#define PS2_COMMAND 0x64

static bool ps2_initialized = false;

bool ps2_dual_channel = false;
uint8_t ps2_ports = 0;

// initializes the playstation 2
bool ps2_init()
{
    ps2_write(0xAD);
    ps2_write(0xA7);

    ps2_read();

    ps2_write(0x20);
    uint8_t config = ps2_read();

    config = BIT_OFF(config, 0);
    config = BIT_OFF(config, 1);
    config = BIT_OFF(config, 6);

    ps2_write(0x60);
    ps2_write_data(config);

    ps2_write(0xAA);
    uint8_t result = ps2_read();

    if (result != 0x55) return false;

    ps2_write(0xA8);
    ps2_write(0x20);
    config = ps2_read();

    ps2_dual_channel = (bool)BIT(config, 5);

    if (ps2_dual_channel)
    {
        ps2_write(0xA7);
        
        config = BIT_OFF(config, 1);
        config = BIT_OFF(config, 5);

        ps2_write(0x60);
        ps2_write_data(config);
    }

    uint8_t ports = 2;

    ps2_write(0xAB);
    result = ps2_read();
    if (result) ports--;

    if (ps2_dual_channel)
    {
        ps2_write(0xA9);
        result = ps2_read();
        if (result) ports--;
    }

    if (!ports) return false;

    ps2_ports = ports;

    ps2_write(0x20);
    config = ps2_read();

    ps2_write(0xAE);
    config = BIT_ON(config, 0);
    if (ps2_dual_channel)
    {
        ps2_write(0xA8);
        config = BIT_ON(config, 1);
    }

    ps2_write(0x60);
    ps2_write_data(config);

    ps2_write_data(0xFF);
    // if (ps2_dual_channel)
    // {
    //     ps2_write(0xD4);
    //     ps2_write_data(0xFF);
    // }

    ps2_initialized = true;
    return true;
}

bool ps2_is_initialized()
{
    return ps2_initialized;
}

uint8_t ps2_read()
{
    return in(PS2_DATA);
}

void ps2_write(uint8_t value)
{
    while (BIT(in(PS2_STATUS), 1));

    out(PS2_COMMAND, value);
}

void ps2_write_data(uint8_t value)
{
    while (BIT(in(PS2_STATUS), 1));

    out(PS2_DATA, value);
}