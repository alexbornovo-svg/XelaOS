#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_BUFFER_SIZE 64

#define SC_CTRL   0x1D
#define SC_ALTGR  0x38
#define SC_Q      0x10
#define SC_S      0x1F

#define SC_UP        0x48
#define SC_DOWN      0x50
#define SC_LEFT      0x4B
#define SC_RIGHT     0x4D
#define SC_BACKSPACE 0x0E

extern volatile uint8_t kbd_buffer[KBD_BUFFER_SIZE];
extern volatile uint8_t kbd_head;
extern volatile uint8_t kbd_tail;

extern uint8_t kbd_ctrl;
extern uint8_t kbd_altgr;

#include "types.h"

void keyboard_init(void);
void keyboard_irq_handler(void);

int keyboard_available(void);
uint8_t keyboard_read_scancode(void);
char kbd_scancode_to_ascii(uint8_t scancode);

uchar kbd_read(void);
void kbd_update(void);
char kbd_poll(void);
uint8_t kbd_getchar(void);
void kbd_flush(void);

#endif