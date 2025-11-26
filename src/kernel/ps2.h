#ifndef PS2_H
#define PS2_H

#include <stdint.h>
#include <stdbool.h>

extern bool ps2_dual_channel;
extern uint8_t ps2_ports;

bool ps2_init();
bool ps2_is_initialized();
uint8_t ps2_read();
void ps2_write(uint8_t value);
void ps2_write_data(uint8_t value);

#endif