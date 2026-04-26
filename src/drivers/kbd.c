#include "types.h"
#include "kstdio.h"
#include "kbd.h"
#include "idt.h"

volatile uint8_t kbd_buffer[KBD_BUFFER_SIZE];
volatile uint8_t kbd_head = 0;
volatile uint8_t kbd_tail = 0;

static uint cursor_line = 0;

void outb(unsigned short port, unsigned char data)
{
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

uchar inb(unsigned short port)
{
    uchar result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static char keymap_ita[128] = {
 /* 0x00 */ 0,
 /* 0x01 */ 27,          
 /* 0x02 */ '1','2','3','4','5','6','7','8','9','0','\'','-',
 /* 0x0E */ '\b',
 /* 0x0F */ '\t',
 /* 0x10 */ 'q','w','e','r','t','y','u','i','o','p',
 /* 0x1A */ '+',         
 /* 0x1B */ 0x00,        
 /* 0x1C */ '\n',
 /* 0x1D */ 0,           /* CTRL           */
 /* 0x1E */ 'a','s','d','f','g','h','j','k','l',
 /* 0x27 */ 'o',         /* ò              */
 /* 0x28 */ 'a',         /* à              */
 /* 0x29 */ '\\',
 /* 0x2A */ 0,           /* LSHIFT         */
 /* 0x2B */ 0,
 /* 0x2C */ 'z','x','c','v','b','n','m',',','.','-',
 /* 0x36 */ 0,           /* RSHIFT         */
 /* 0x37 */ '*',
 /* 0x38 */ 0,           /* ALT            */
 /* 0x39 */ ' ',
};

char kbd_scancode_to_ascii(uchar scancode)
{
    if (scancode >= 128) return 0;
    return keymap_ita[scancode];
}

static int kbd_data_ready()
{
    return inb(0x64) & 0x01;
}

char kbd_poll(void)
{
    if (!kbd_data_ready())
    {
        return 0;
    }

    uchar scancode = inb(0x60);

    if (scancode & 0x80)
    {
        return 0;
    }

    return kbd_scancode_to_ascii(scancode);
}

char kbd_char_input(void)
{
    char c;
    do {
        c = kbd_poll();
    } while (c == 0);

    return c;
}

void kbd_update(void)
{
    if (kbd_head == kbd_tail) return;

    uint8_t sc = kbd_buffer[kbd_tail];
    kbd_tail = (kbd_tail + 1) % KBD_BUFFER_SIZE;

    if (sc & 0x80) return;

    char c = kbd_scancode_to_ascii(sc);
    if (c) {
        char str[2] = {c, '\0'};
        cursor_line = kwrite(str, cursor_line, WHITE);
    }
}

uchar kbd_read(void)
{
    while (1)
    {
        __asm__ volatile ("cli");
        if (kbd_head != kbd_tail) break;
        __asm__ volatile ("sti");
        __asm__ volatile ("hlt");
    }

    uint8_t sc = kbd_buffer[kbd_tail];
    kbd_tail = (kbd_tail + 1) % KBD_BUFFER_SIZE;
    __asm__ volatile ("sti");
    return sc;
}