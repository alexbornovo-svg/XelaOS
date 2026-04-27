#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "types.h"

typedef struct {
    char text[40];
    int clock_on;
    uint8_t fg;
    uint8_t bg;
} statusbar_state_t;

extern statusbar_state_t statusbar_state;

void statusbar_update(const char *text, int clock_on, uint8_t fg_color, uint8_t bg_color);
void statusbar_refresh(void);

#endif