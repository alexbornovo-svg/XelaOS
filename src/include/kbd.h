#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_BUFFER_SIZE 64

#include "types.h"

void keyboard_init(void);
void keyboard_irq_handler(void);

int keyboard_available(void);
uint8_t keyboard_read_scancode(void);

#endif