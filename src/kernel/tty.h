#ifndef TTY_H
#define TTY_H

typedef void (*tty_putc_t)(char);
typedef void (*tty_puts_t)(const char *);

void tty_set_putc(tty_putc_t function);
void tty_set_puts(tty_puts_t function);

void tty_putc(char c);
void tty_puts(const char *s);

#endif