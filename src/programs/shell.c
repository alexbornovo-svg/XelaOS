#include "types.h"
#include "kstdio.h"
#include "ata_pio.h"

int shell(int line)
{
    static uint8_t sb_buffer[1024];
    uchar cmd[256] = {0};
    while(1)
    {
        line = kinput("> ", line, WHITE, cmd, 256);
        if (startswith(cmd, "cls"))
        {
            kclear();
            line = 0;
        }
        else if (startswith(cmd, "fetch"))
        {
            line = kwrite("XELAOS 1.0\n", line, WHITE);
            line = kwrite("Programmed by Alex Xela (alexbornovo-svg)", line, LIGHT_GREY);
            line = kwrite("1. This is a green line", line, GREEN);
            line = kwrite("1. This is a red line", line, RED);
            line = kwrite("1. This is a blue line", line, BLUE);

            line++;

            char brand[49];
            cpu_brand(brand);

            line = kwrite("CPU Model and Brand:", line, WHITE);
            line = kwrite(brand, line, LIGHT_RED);
            line++;

            uint32_t f_mb, t_mb;
            get_disk_usage(global_sb, &f_mb, &t_mb);

            char b1[12], b2[12];
            itoa(f_mb, b1);
            itoa(t_mb, b2);

            char disk_buf[VGA_WIDTH + 1];
            disk_buf[0] = '\0';
            kstrcat(disk_buf, "Free space: ");
            kstrcat(disk_buf, b1);
            kstrcat(disk_buf, " MB / ");
            kstrcat(disk_buf, b2);
            kstrcat(disk_buf, " MB");

            line = kwrite(disk_buf, line, WHITE);
            line++;
        }
        else if (startswith(cmd, "ext2"))
        {
            if (startswith(cmd, "ext2 -init"))
            {
                line = ext2_init(line);
            }
            else
            {
                line = kwrite("ERROR - ext2 command are: -init", line, RED);
            }
        }
        else if (startswith(cmd, "test"))
        {
            if (startswith(cmd, "test -scroll"))
            {
                for (int i = 0; i<30; i++)
                {
                    line = kwrite("Scroll testing", line, WHITE);
                }
            }
            else if ((startswith(cmd, "test -")))
            {

            }
        }
        else
        {
            line = kwrite("ERROR: cmd not found", line, RED);
        }
    }

    return 0;
}

/*
    uchar cmd[256] = {0};
    uint line = 0;
    
    kclear();
    line = kwrite("Welcome to the kernel", line, WHITE);

    while (1)
    {
        line = kinput("> ", line, WHITE, cmd, 256);
    }
*/