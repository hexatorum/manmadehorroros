#ifndef TTY_H
#define TTY_H

#include <stddef.h>

typedef void (*tty_putc_t)(char);

void tty_set_putc(tty_putc_t function);

void tty_putc(char c);
void tty_puts(const char *s);
void tty_putsn(const char *s, size_t n);

#endif