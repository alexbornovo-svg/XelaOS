#include "idt.h"
#include "kbd.h"
#include "io.h"
#include "isr.h"
#include "statusbar.h"
#include "kstdio.h"

static uint32_t timer_ticks = 0;
#define PIT_HZ 100
#define SC_CTRL    0x1D
#define SC_ALTGR   0x38 

void irq_handler(registers_t *regs)
{
    if (regs->int_no == 32)
    {
        timer_ticks++;
        if (timer_ticks % PIT_HZ == 0)
        {
            statusbar_refresh();
        }
    }
    else if (regs->int_no == 33)
        {
            uint8_t scancode = inb(0x60);

            if (scancode == SC_CTRL)        kbd_ctrl  = 1;
            if (scancode == SC_CTRL  + 0x80) kbd_ctrl  = 0;
            if (scancode == SC_ALTGR)       kbd_altgr = 1;
            if (scancode == SC_ALTGR + 0x80) kbd_altgr = 0;

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