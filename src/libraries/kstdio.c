#include "types.h"
#include "kstdio.h"
#include "kbd.h"

void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

const char *skipword(const char *str)
{
    while (*str && *str != ' ') str++;
    while (*str == ' ') str++;
    return str;
}

int startswith(const char *str, const char *prefix)
{
    int i = 0;
    while (prefix[i])
    {
        if (str[i] != prefix[i]) return 0;
        i++;
    }
    return 1;
}

uint kwrite(char *message, uint line, uchar colour)
{
    char *vidmem = VGA_MEM;

    if (line >= VGA_HEIGHT)
    {
        kscroll();
        line = VGA_HEIGHT - 1;
    }
    unsigned int i = line * VGA_WIDTH * 2;

    while (*message != '\0')
    {
        if (*message == '\n')
        {
            line++;
            if (line >= VGA_HEIGHT)
            {
                kscroll();
                line = VGA_HEIGHT - 1;
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
        while (!(inb(0x64) & 0x01));
        uchar scancode = inb(0x60);
        if (scancode & 0x80) 
        {
            continue;
        }

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

void kscroll()
{
    char *vidmem = VGA_MEM;
    unsigned int i;
    for (i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH * 2; i++)
        vidmem[i] = vidmem[i + VGA_WIDTH * 2];
    for (i = (VGA_HEIGHT - 1) * VGA_WIDTH * 2; i < VGA_HEIGHT * VGA_WIDTH * 2; i += 2)
    {
        vidmem[i]     = ' ';
        vidmem[i + 1] = 0x07;
    }
}

void cpu_brand(char *buf)
{
    unsigned int eax, ebx, ecx, edx;
    int i, j = 0;

    unsigned int regs[3][4];
    for (i = 0; i < 3; i++)
    {
        eax = 0x80000002 + i;
        __asm__ volatile (
            "cpuid"
            : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
            : "a"(eax)
        );
        regs[i][0] = eax;
        regs[i][1] = ebx;
        regs[i][2] = ecx;
        regs[i][3] = edx;
    }

    for (i = 0; i < 3; i++)
    {
        int k;
        for (k = 0; k < 4; k++)
        {
            unsigned int r = regs[i][k];
            buf[j++] = (r      ) & 0xFF;
            buf[j++] = (r >>  8) & 0xFF;
            buf[j++] = (r >> 16) & 0xFF;
            buf[j++] = (r >> 24) & 0xFF;
        }
    }
    buf[j] = '\0';
}

void get_disk_usage(ext2_superblock_t *sb, uint32_t *free_mb, uint32_t *total_mb) {
    uint32_t block_size_kb = (1024 << sb->log_block_size) / 1024;

    *free_mb = (sb->free_blocks_count * block_size_kb) / 1024;
    *total_mb = (sb->blocks_count * block_size_kb) / 1024;
}

void itoa(uint32_t n, char *str) {
    char temp[12];
    int i = 0;

    if (n == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    while (n > 0) {
        temp[i++] = (n % 10) + '0';
        n /= 10;
    }

    int j;
    for (j = 0; j < i; j++) {
        str[j] = temp[i - 1 - j];
    }
    str[i] = '\0';
}

void kstrcat(char *dst, const char *src)
{
    while (*dst) dst++;
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

void kmemcpy(uint8_t *dst, const uint8_t *src, uint32_t n)
{
    while (n--) *dst++ = *src++;
}