#ifndef KSTDIO_H
#define KSTDIO_H 

#include "types.h"
#include "kbd.h"

#define VGA_MEM ((char *) 0xB8000)
#define VGA_WIDTH 80U
#define VGA_HEIGHT 25U

#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define MAGENTA 0x5
#define BROWN 0x6
#define LIGHT_GREY 0x7
#define DARK_GREY 0x8
#define LIGHT_BLUE 0x9
#define LIGHT_GREEN 0xA
#define LIGHT_CYAN 0xB
#define LIGHT_RED 0xC
#define LIGHT_MAGENTA 0xD
#define LIGHT_BROWN 0xE
#define WHITE 0xF

uint kwrite(const char *message, uint line, uchar colour);
void kclear();
uint kinput(const char *message, uint line, char colour, char *buf, uint buf_size);
void k_scroll();

#endif