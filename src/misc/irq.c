#include "idt.h"
#include "kbd.h"
#include "io.h"
#include "types.h"
#include "isr.h"

void irq_handler(registers_t *regs)
{

    if (regs->int_no == 33) {
        uint8_t scancode = inb(0x60);
        if (!(scancode & 0x80))
        {
            uint8_t next = (kbd_head + 1) % KBD_BUFFER_SIZE;
            if (next != kbd_tail)
            {
                kbd_buffer[kbd_head] = scancode;
                kbd_head = next;
            }
        }
    }
    if (regs->int_no >= 40)
        port_byte_out(0xA0, 0x20);
    port_byte_out(0x20, 0x20);
}