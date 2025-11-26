#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

typedef enum {
    CONSOLE_FG_BLACK = 0,
    CONSOLE_FG_BLUE,
    CONSOLE_FG_GREEN,
    CONSOLE_FG_CYAN,
    CONSOLE_FG_RED,
    CONSOLE_FG_MAGENTA,
    CONSOLE_FG_BROWN,
    CONSOLE_FG_GRAY,
    CONSOLE_FG_BRIGHT
} console_fg_t;

typedef enum {
    CONSOLE_BG_BLACK = 0,
    CONSOLE_BG_BLUE = 1 << 4,
    CONSOLE_BG_GREEN = 2 << 4,
    CONSOLE_BG_CYAN = 3 << 4,
    CONSOLE_BG_RED = 4 << 4,
    CONSOLE_BG_MAGENTA = 5 << 4,
    CONSOLE_BG_BROWN = 6 << 4,
    CONSOLE_BG_GRAY = 7 << 4
} console_bg_t;

void console_clear();
void console_set_style(uint8_t style);
void console_reset_style();
void console_puts(const char *string);

#endif