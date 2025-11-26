#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <cpuid.h>

#include <arch/io.h>
#include <arch/gdt.h>
#include <arch/interrupts.h>
#include <arch/irq.h>
#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/console.h>
#include <lib/memory.h>

#define MAX_BUFFER_SIZE 256

const uint8_t SUCCESS = CONSOLE_FG_BRIGHT | CONSOLE_FG_GREEN;
const uint8_t WARNING = CONSOLE_FG_BRIGHT | CONSOLE_FG_BROWN;
const uint8_t ERROR = CONSOLE_FG_BRIGHT | CONSOLE_BG_RED;

static char buffer[MAX_BUFFER_SIZE + 1];
static size_t cursor_position = 0;
static size_t buffer_position = 0;

static const char LOWER_CODE_LOOKUP[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+'
};

static const char UPPER_CODE_LOOKUP[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
    0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+'
};

static void handle_key_down(uint8_t code, bool capital, bool extended)
{
    const char *lookup = capital ? UPPER_CODE_LOOKUP : LOWER_CODE_LOOKUP;

    if (!lookup[code]) return;

    char string[] = { lookup[code], 0 };

    console_puts(string);
}

void main()
{   
    // console_puts("terminal initialized\n");
    // console_puts("loading gdt... ");

    gdt_init();

    // console_set_style(SUCCESS);
    // console_puts("done\n");
    // console_reset_style();

    // console_puts("loading idt... ");

    idt_init();

    // console_set_style(SUCCESS);
    // console_puts("done\n");
    // console_reset_style();

    // console_puts("initializing irqs... ");

    irq_init(32);

    // console_set_style(SUCCESS);
    // console_puts("done\n");
    // console_reset_style();
    
    interrupts_enable();

    // console_puts("initializing serial... ");

    // if (!serial_init())
    // {
    //     console_set_style(WARNING);
    //     console_puts("FAILED, will not be used\n");
    // }
    // else
    // {
    //     console_set_style(SUCCESS);
    //     console_puts("done\n");  
    // }
    // console_reset_style();

    // console_puts("initializing ps/2... ");

    // if (!ps2_init())
    // {
    //     // console_set_style(ERROR);
    //     // console_puts("FAILED, halting\n");
    //     return;
    // }

    // console_set_style(SUCCESS);
    // console_puts("done\n");
    // console_reset_style();

    // console_puts("initializing keyboard... ");

    // if (!keyboard_init())
    // {
    //     // console_set_style(ERROR);
    //     // console_puts("FAILED, halting\n");
    //     return;
    // }

    // keyboard_set_down_handler(handle_key_down);

    // console_set_style(SUCCESS);
    // console_puts("done\n");
    // console_reset_style();

    tty_set_putc(NULL);
    tty_set_puts(serial_puts);

    tty_puts("\x1B[2J\x1B[;H");
    tty_puts("manmadehorrorOS -1.0\n");
    tty_puts("copyleft (C) 2025 man-made horrors beyond your comprehension corporation\n\n");

    while (true)
    {
        tty_puts("$ ");
        
        while (true); // replacement for wait for input
    }
}