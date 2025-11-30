#include <kernel/tty.h>

static tty_putc_t putc;

void tty_set_putc(tty_putc_t function)
{
    putc = function;
}

void tty_putc(char c)
{
    if (putc) putc(c);
}

void tty_puts(const char *s)
{
    if (!putc) return;
    
    while (*s)
    {
        putc(*s++);
    }
}

void tty_putsn(const char *s, size_t n)
{
    if (!putc) return;

    while (n--)
    {
        putc(*s++);
    }
}