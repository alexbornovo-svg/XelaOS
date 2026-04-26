#ifndef STRING_H
#define STRING_H

#include "types.h"

const char *skipword(const char *str);
int startswith(const char *str, const char *prefix);
uint kstrlen(const char *s);
void kstrcpy(char *dst, const char *src);
int kstrcmp(const char *a, const char *b);
void kstrcat(char *dst, const char *src);
void itoa(uint32_t n, char *str);
void kmemset(uint8_t *dst, uint8_t val, uint32_t n);
void kmemcpy(uint8_t *dst, const uint8_t *src, uint32_t n);
int kmemcmp(const uint8_t *a, const uint8_t *b, uint32_t n);

#endif