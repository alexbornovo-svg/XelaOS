#include "types.h"
#include "kstdio.h"
#include "isr.h"
#include "kbd.h"

void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

void kscroll()
{
    char *vidmem = VGA_MEM;
    unsigned int i;
    for (i = 0; i < 23 * VGA_WIDTH * 2; i++)
        vidmem[i] = vidmem[i + VGA_WIDTH * 2];
    for (i = 23 * VGA_WIDTH * 2; i < 24 * VGA_WIDTH * 2; i += 2)
    {
        vidmem[i]     = ' ';
        vidmem[i + 1] = 0x07;
    }
}

uint kwrite(const char *message, uint line, uchar colour)
{
    char *vidmem = VGA_MEM;
    if (line >= 24)  // riga 24 riservata alla statusbar
    {
        kscroll();
        line = 23;
    }
    unsigned int i = line * VGA_WIDTH * 2;
    while (*message != '\0')
    {
        if (*message == '\n')
        {
            line++;
            if (line >= 24)
            {
                kscroll();
                line = 23;
            }
            i = line * VGA_WIDTH * 2;
        }
        else
        {
            vidmem[i++] = *message;
            vidmem[i++] = colour;
        }
        message++;
    }
    return line + 1;
}

void kclear(void)
{
    char *vidmem = VGA_MEM;
    unsigned int i = 0;
    while (i < (VGA_WIDTH * VGA_HEIGHT * 2))
    {
        vidmem[i++] = ' ';
        vidmem[i++] = 0x07;
    }
    port_byte_out(0x3D4, 14);
    port_byte_out(0x3D5, 0);
    port_byte_out(0x3D4, 15);
    port_byte_out(0x3D5, 0);
}

uint kinput(const char *message, uint line, char colour, char *buf, uint buf_size)
{
    if (!buf || buf_size == 0)
    {
        return 0;
    }
    uint msg_len = 0;

    while (message[msg_len])
    {
        msg_len++;
    }
    uint id = 0;
    char tmp[VGA_WIDTH + 1];

    kwrite(message, line, colour);
    while (id < buf_size - 1)
    {
        uchar scancode = kbd_read();

        char c = kbd_scancode_to_ascii(scancode);
        if (c == 0)
        {
            continue;
        }
        if (c == '\b')
        {
            if (id > 0) 
            {
                id--;
            }
            buf[id] = '\0';
        }
        else if (c == '\n')
        {
            break;
        }
        else 
        {
            buf[id++] = c;
        }
        uint i = 0;
        for (uint j = 0; j < msg_len && i < VGA_WIDTH; j++)
        {
            tmp[i++] = message[j];
        }
        for (uint j = 0; j < id && i < VGA_WIDTH; j++)
        {
            tmp[i++] = buf[j];
        }
        while (i < VGA_WIDTH)
        {
            tmp[i++] = ' ';
        }
        tmp[i] = '\0';
        kwrite(tmp, line, colour);
    }
    buf[id] = '\0';
    return line + 1;
}