#include "types.h"

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

uint kstrlen(const char *s)
{
    uint n = 0;
    while (s[n]) n++;
    return n;
}

void kstrcpy(char *dst, const char *src)
{
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

int kstrcmp(const char *a, const char *b)
{
    while (*a && *a == *b) { a++; b++; }
    return *a - *b;
}

void kstrcat(char *dst, const char *src)
{
    while (*dst) dst++;
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

void itoa(uint32_t n, char *str)
{
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
    for (j = 0; j < i; j++)
        str[j] = temp[i - 1 - j];
    str[i] = '\0';
}

void kmemset(uint8_t *dst, uint8_t val, uint32_t n)
{
    while (n--) *dst++ = val;
}

void kmemcpy(uint8_t *dst, const uint8_t *src, uint32_t n)
{
    while (n--) *dst++ = *src++;
}

int kmemcmp(const uint8_t *a, const uint8_t *b, uint32_t n)
{
    while (n--)
    {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return 0;
}