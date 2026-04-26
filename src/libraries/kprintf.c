#include "kprintf.h"
#include "kstdio.h"
#include "types.h"

static uint _kstrlen(const char *s)
{
    uint n = 0;
    while (s[n]) n++;
    return n;
}

static uint _append_str(char *buf, uint pos, uint max, const char *s)
{
    while (*s && pos < max - 1)
        buf[pos++] = *s++;
    return pos;
}

static uint _append_uint(char *buf, uint pos, uint max, uint32_t n, uint base, int uppercase)
{
    char tmp[16];
    int  i = 0;

    if (n == 0)
    {
        if (pos < max - 1) buf[pos++] = '0';
        return pos;
    }

    while (n > 0)
    {
        uint32_t rem = n % base;
        if (rem < 10)
            tmp[i++] = '0' + rem;
        else
            tmp[i++] = (uppercase ? 'A' : 'a') + (rem - 10);
        n /= base;
    }

    while (i > 0 && pos < max - 1)
        buf[pos++] = tmp[--i];

    return pos;
}

static uint _append_int(char *buf, uint pos, uint max, int32_t n)
{
    if (n < 0)
    {
        if (pos < max - 1) buf[pos++] = '-';
        return _append_uint(buf, pos, max, (uint32_t)(-n), 10, 0);
    }
    return _append_uint(buf, pos, max, (uint32_t)n, 10, 0);
}

uint kprintf(uint line, uchar colour, const char *fmt, ...)
{
    char buf[VGA_WIDTH + 1];
    uint pos = 0;

    uint32_t *arg = (uint32_t *)(&fmt) + 1;

    const char *p = fmt;
    while (*p && pos < VGA_WIDTH)
    {
        if (*p != '%')
        {
            if (*p == '\n')
            {
                buf[pos] = '\0';
                line = kwrite(buf, line, colour);
                pos = 0;
                p++;
                continue;
            }
            buf[pos++] = *p++;
            continue;
        }

        p++; /* salta '%' */
        switch (*p)
        {
            case 's':
            {
                const char *s = (const char *)(*arg++);
                if (!s) s = "(null)";
                pos = _append_str(buf, pos, VGA_WIDTH + 1, s);
                break;
            }
            case 'd':
                pos = _append_int(buf, pos, VGA_WIDTH + 1, (int32_t)(*arg++));
                break;

            case 'u':
                pos = _append_uint(buf, pos, VGA_WIDTH + 1,
                                   *arg++, 10, 0);
                break;

            case 'x':
                pos = _append_uint(buf, pos, VGA_WIDTH + 1,
                                   *arg++, 16, 0);
                break;

            case 'X':
                pos = _append_uint(buf, pos, VGA_WIDTH + 1,
                                   *arg++, 16, 1);
                break;

            case 'c':
                if (pos < VGA_WIDTH)
                    buf[pos++] = (char)(*arg++);
                break;

            case '%':
                if (pos < VGA_WIDTH)
                    buf[pos++] = '%';
                break;

            default:
                if (pos < VGA_WIDTH) buf[pos++] = '%';
                if (pos < VGA_WIDTH) buf[pos++] = *p;
                break;
        }
        p++;
    }

    buf[pos] = '\0';
    return kwrite(buf, line, colour);
}