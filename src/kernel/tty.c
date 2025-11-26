#include <kernel/tty.h>

static tty_putc_t putc;
static tty_puts_t puts;

void tty_set_putc(tty_putc_t function)
{
    putc = function;
}

void tty_set_puts(tty_puts_t function)
{
    puts = function;
}

void tty_putc(char c)
{
    if (putc) putc(c);
}

void tty_puts(const char *s)
{
    if (puts) puts(s);
}