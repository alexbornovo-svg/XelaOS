#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_BUFFER_SIZE 64

extern volatile uint8_t kbd_buffer[KBD_BUFFER_SIZE];
extern volatile uint8_t kbd_head;
extern volatile uint8_t kbd_tail;

#include "types.h"

void keyboard_init(void);
void keyboard_irq_handler(void);

int keyboard_available(void);
uint8_t keyboard_read_scancode(void);
char kbd_scancode_to_ascii(uint8_t scancode);

uchar kbd_read(void);
void kbd_update(void);
char kbd_poll(void);

#endif