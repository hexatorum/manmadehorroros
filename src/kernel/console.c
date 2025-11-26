#include <kernel/console.h>

#include <arch/io.h>
#include <lib/memory.h>

#define WIDTH 80
#define HEIGHT 25

const uint8_t DEFAULT_STYLE = CONSOLE_BG_BLACK | CONSOLE_FG_GRAY;

static uint8_t column = 0, row = 0;

static uint16_t *buffer = (uint16_t *)0xB8000;
static uint8_t style = DEFAULT_STYLE;

static void console_scroll()
{
    for (int i = 1; i < HEIGHT; i++)
    {
        memcpy(
            &buffer[(i - 1) * WIDTH],
            &buffer[i * WIDTH],
            WIDTH * sizeof(buffer[0])
        );
    }

    // footgun-inator 3000 (may or may not be a problem)
    memset(&buffer[(HEIGHT - 1) * WIDTH], 0, WIDTH * sizeof(buffer[0]), 1);
    memset(&buffer[(HEIGHT - 1) * WIDTH] + 1, style, WIDTH * sizeof(buffer[0]), 1);
}

static void console_new_line()
{   
    row++;
    column = 0;

    if (row == HEIGHT)
    {
        console_scroll();
        row--;
    }
}

static void console_put_char(char c)
{
    switch (c)
    {
        case '\n': console_new_line(); break;
        case '\r': column = 0; break;
        default: {
            buffer[row * WIDTH + column] = c | (style << 8);
            column++;
        }
    }
}

static void console_update_cursor()
{
    uint16_t position = row * WIDTH + column;
    
    out(0x3D4, 0x0F);
	out(0x3D5, (uint8_t)(position & 0xFF));
	out(0x3D4, 0x0E);
	out(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}

void console_clear()
{
    column = 0; row = 0;

    // footgun-inator 4000
    memset(buffer, 0, WIDTH * HEIGHT * sizeof(buffer[0]), 1);
    memset(buffer, style, WIDTH * HEIGHT * sizeof(buffer[0]), 1);
}

void console_set_style(uint8_t new)
{
    style = new;
}

void console_reset_style()
{
    style = DEFAULT_STYLE;
}

void console_puts(const char *string)
{
    while (*string)
    {
        console_put_char(*string++);
    }

    console_update_cursor();
}