#include "types.h"
#include "kstdio.h"
#include "time.h"

#define STATUS_ROW 24

typedef struct {
    char     text[40];
    int      clock_on;
    uint8_t  fg;
    uint8_t  bg;
} statusbar_state_t;

statusbar_state_t statusbar_state = {
    .text     = "Terminal",
    .clock_on = 1,
    .fg       = 0x07,
    .bg       = 0x01
};

void statusbar_refresh(void)
{
    uint8_t color = (statusbar_state.bg << 4) | (statusbar_state.fg & 0x0F);
    char *vidmem = (char *)0xB8000;
    int base = STATUS_ROW * 80 * 2;

    for (int i = 0; i < 80; i++) {
        vidmem[base + i * 2]     = ' ';
        vidmem[base + i * 2 + 1] = color;
    }

    const char *name = statusbar_state.text;
    for (int i = 0; name[i]; i++) {
        vidmem[base + i * 2]     = name[i];
        vidmem[base + i * 2 + 1] = color;
    }

    if (statusbar_state.clock_on) {
        cmos_time_t t;
        cmos_read(&t);
        char time_str[9];
        time_str[0] = '0' + (t.hours   / 10);
        time_str[1] = '0' + (t.hours   % 10);
        time_str[2] = ':';
        time_str[3] = '0' + (t.minutes / 10);
        time_str[4] = '0' + (t.minutes % 10);
        time_str[5] = ':';
        time_str[6] = '0' + (t.seconds / 10);
        time_str[7] = '0' + (t.seconds % 10);
        time_str[8] = '\0';

        int col = 72;
        for (int i = 0; i < 8; i++) {
            vidmem[base + (col + i) * 2]     = time_str[i];
            vidmem[base + (col + i) * 2 + 1] = color;
        }
    }
}

void statusbar_update(const char *text, int clock_on, uint8_t fg_color, uint8_t bg_color)
{
    int i;
    for (i = 0; text[i] && i < 39; i++)
        statusbar_state.text[i] = text[i];
    statusbar_state.text[i]  = '\0';
    statusbar_state.clock_on = clock_on;
    statusbar_state.fg       = fg_color;
    statusbar_state.bg       = bg_color;

    statusbar_refresh();
}