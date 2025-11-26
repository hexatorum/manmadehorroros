#include <kernel/keyboard.h>

#include <arch/interrupts.h>
#include <kernel/ps2.h>
#include <kernel/serial.h>

#define MAX_BUFFER_SIZE 256

static const char HEX_LOOKUP[16] = "0123456789ABCDEF";

static bool keyboard_initialized = false;
static keyboard_event_t event_buffer[MAX_BUFFER_SIZE] = {};
static size_t buffer_position = 0;
static keyboard_modifiers_t modifiers;

static keyboard_handler_t down_handler;
static keyboard_handler_t up_handler;

static void keyboard_handle_down(uint8_t code, bool extended)
{
    switch (code)
    {
        case 0x1D: modifiers.control = true; break;
        case 0x2A: modifiers.shift = true; break;
        case 0x38: modifiers.alt = true; break;
        case 0x3A: modifiers.caps = !modifiers.caps; break;
        default: {
            if (down_handler)
                down_handler(code, modifiers.shift != modifiers.caps, extended);

            break;
        }
    }
}

static void keyboard_handle_up(uint8_t code, bool extended)
{
    switch (code)
    {
        case 0x1D: modifiers.control = false; break;
        case 0x2A: modifiers.shift = false; break;
        case 0x38: modifiers.alt = false; break;
        case 0x3A: break;
        default: {
            if (up_handler)
                up_handler(code, modifiers.shift != modifiers.caps, extended);

            break;
        }
    }
}

static void keyboard_irq_handler()
{
    uint8_t code = ps2_read();

    bool extended = code == 0xE0;
    if (extended)
        code = ps2_read();

    bool up = code >> 7;
    code &= 0x7F;

    if (!up)
        keyboard_handle_down(code, extended);
    else
        keyboard_handle_up(code, extended);

    serial_puts(modifiers.shift || modifiers.caps ? "capsy locky\n" : "no caps?\n");
}

bool keyboard_init()
{
    // if (!ps2_is_initialized()) return false;

    interrupts_set(0x21, keyboard_irq_handler);

    keyboard_initialized = true;
    return true;
}

void keyboard_set_down_handler(keyboard_handler_t handler)
{
    down_handler = handler;
}