#include "arch/x86/paging.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <cpuid.h>

#include <arch/x86/io.h>
#include <arch/x86/gdt.h>
#include <arch/x86/interrupts.h>
#include <arch/x86/pic.h>
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
    gdt_init();

    idt_init();
    irq_init(32);
    interrupts_enable();

    paging_init();
    
    serial_init();

    tty_set_putc(serial_putc);

    tty_putsn("GDT init... OK", 12);
    tty_puts("uhhhh next question\n\n");
    tty_puts("manmadehorrorOS -1.0\n");
    tty_puts("copyleft (C) 2025 man-made horrors beyond your comprehension corporation\n\n");

    while (true) 
    {
        tty_puts("$ ");

        console_puts("kurwa");

        while (true); // replacement for wait for input
    }
}