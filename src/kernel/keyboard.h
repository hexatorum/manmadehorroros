#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool control : 1;
    bool alt : 1;
    bool shift : 1;
    bool caps : 1;
} keyboard_modifiers_t;

typedef struct {
    uint8_t code;
} keyboard_event_t;

typedef void (*keyboard_handler_t)(uint8_t, bool, bool);

bool keyboard_init();
void keyboard_set_down_handler(keyboard_handler_t handler);

#endif