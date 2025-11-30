#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

bool serial_init();
bool serial_is_initialized();
void serial_putc(char c);

#endif