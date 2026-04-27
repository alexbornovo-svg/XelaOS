#include "types.h"
#include "kprintf.h"
#include "kstdio.h"
#include "statusbar.h"
#include "string.h"
#include "kbd.h"
#include "time.h"

#define CMD_ROW     23
#define VISIBLE_ROWS 23
#define MAX_ROWS    256
#define MAX_COLS     79

static char document[MAX_ROWS][MAX_COLS + 1];
static int  doc_lines = MAX_ROWS;

static int  cursor_row = 0;
static int  cursor_col = 0;
static int  scroll_top = 0;

static uint32_t tick = 0;
static int  cursor_visible = 1;

static void doc_init(void)
{
    for (int i = 0; i < MAX_ROWS; i++)
    {
        for (int j = 0; j < MAX_COLS; j++)
            document[i][j] = ' ';
        document[i][MAX_COLS] = '\0';
    }
}

static void render_screen(void)
{
    char *vidmem = (char *)0xB8000;

    for (int row = 0; row < VISIBLE_ROWS; row++)
    {
        int doc_row = scroll_top + row;
        int base = row * 80 * 2;

        char num[4];
        int n = doc_row + 1;
        num[0] = '0' + (n / 100);
        num[1] = '0' + ((n / 10) % 10);
        num[2] = '0' + (n % 10);
        num[3] = '\0';

        vidmem[base + 0] = num[0]; vidmem[base + 1] = DARK_GREY;
        vidmem[base + 2] = num[1]; vidmem[base + 3] = DARK_GREY;
        vidmem[base + 4] = num[2]; vidmem[base + 5] = DARK_GREY;
        vidmem[base + 6] = ' ';   vidmem[base + 7] = DARK_GREY;

        for (int col = 0; col < MAX_COLS - 3; col++)
        {
            char ch = document[doc_row][col];
            uint8_t attr = (BLACK << 4) | (BLACK & 0x0F);

            int is_cursor = (doc_row == cursor_row) && (col == cursor_col);
            if (is_cursor && cursor_visible)
            {
                ch   = '#';
                attr = (WHITE << 4) | (WHITE & 0x0F);
            }

            int screen_col = col + 4;
            vidmem[base + screen_col * 2]     = ch;
            vidmem[base + screen_col * 2 + 1] = attr;
        }
    }
}

static void draw_cmdbar(const char *text, uint8_t fg, uint8_t bg)
{
    char *vidmem = (char *)0xB8000;
    int base = CMD_ROW * 80 * 2;
    uint8_t color = (bg << 4) | (fg & 0x0F);
    for (int i = 0; i < 80; i++) {
        vidmem[base + i * 2]     = ' ';
        vidmem[base + i * 2 + 1] = color;
    }
    for (int i = 0; text[i]; i++) {
        vidmem[base + i * 2]     = text[i];
        vidmem[base + i * 2 + 1] = color;
    }
}

static void clear_cmdbar(void)
{
    char *vidmem = (char *)0xB8000;
    int base = CMD_ROW * 80 * 2;
    uint8_t color = (BLACK << 4) | (WHITE & 0x0F);
    for (int i = 0; i < 80; i++) {
        vidmem[base + i * 2]     = ' ';
        vidmem[base + i * 2 + 1] = color;
    }
}

int xte_terminal()
{
    kbd_flush();
    kclear();
    doc_init();

    int cmd_mode = 0;
    cursor_row = 0;
    cursor_col = 0;
    scroll_top = 0;
    tick = 0;
    cursor_visible = 1;

    statusbar_update("Xela Integ Editor", 1, BLACK, LIGHT_GREY);
    render_screen();

    while (1)
    {
        __asm__ volatile ("hlt");

        tick++;
        if (tick % 50 == 0)
        {
            cursor_visible = !cursor_visible;
            render_screen();
        }

        uint8_t sc = kbd_getchar();
        if (!sc) continue;
        if (sc == SC_CTRL || sc == SC_ALTGR) continue;

        if (!cmd_mode)
        {
            if (kbd_ctrl && kbd_altgr)
            {
                cmd_mode = 1;
                kbd_flush();
                draw_cmdbar(":  q=esci  s=salva", WHITE, DARK_GREY);
                continue;
            }

            if (sc == SC_UP)
            {
                if (cursor_row > 0) cursor_row--;
                if (cursor_row < scroll_top) scroll_top--;
            }
            else if (sc == SC_DOWN)
            {
                if (cursor_row < MAX_ROWS - 1) cursor_row++;
                if (cursor_row >= scroll_top + VISIBLE_ROWS) scroll_top++;
            }
            else if (sc == SC_LEFT)
            {
                if (cursor_col > 0) cursor_col--;
            }
            else if (sc == SC_RIGHT)
            {
                if (cursor_col < MAX_COLS - 4) cursor_col++;
            }
            else if (sc == SC_BACKSPACE)
            {
                if (cursor_col > 0)
                {
                    cursor_col--;
                    document[cursor_row][cursor_col] = ' ';
                }
            }
            else
            {
                char ch = sc_to_char(sc);
                if (ch && cursor_col < MAX_COLS - 4)
                {
                    document[cursor_row][cursor_col] = ch;
                    cursor_col++;
                }
            }

            render_screen();
        }
        else
        {
            if (sc == SC_Q)
            {
                kbd_flush();
                return 0;
            }
            else if (sc == SC_S)
            {
                draw_cmdbar("SAVED", GREEN, BLACK);
                cmos_time_t t1, t2;
                cmos_read(&t1);
                do { cmos_read(&t2); }
                while (((t2.seconds - t1.seconds + 60) % 60) < 2);
                clear_cmdbar();
                cmd_mode = 0;
                render_screen();
            }
            else
            {
                cmd_mode = 0;
                clear_cmdbar();
                render_screen();
            }
        }
    }
}