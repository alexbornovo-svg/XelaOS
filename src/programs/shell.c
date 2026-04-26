#include "types.h"
#include "kstdio.h"
#include "ata_pio.h"
#include "string.h"
#include "disk.h"
#include "cpu.h"
#include "time.h"
#include "kprintf.h"
#include "random.h"

int shell(int line)
{
    static uint8_t sb_buffer[1024];
    char cmd[256] = {0};
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

            line = kprintf(line, LIGHT_RED, "CPU: %s", brand);
            line++;

            uint32_t f_mb, t_mb;
            get_disk_usage(global_sb, &f_mb, &t_mb);

            char b1[12], b2[12];
            itoa(f_mb, b1);
            itoa(t_mb, b2);

            char disk_buf[VGA_WIDTH + 1];
            disk_buf[0] = '\0';
            line = kprintf(line, 0x07, "Disk: %u MB free of %u MB", f_mb, t_mb);

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
            else
            
            {
                line = kwrite("ERROR - test command are: -scroll", line, RED);
            }
        }
        else if (startswith(cmd, "time"))
        {
            cmos_time_t t;
            cmos_read(&t);
            line = kprintf(line, LIGHT_GREY, "Date: %u/%u/20%u", t.day, t.month, t.year);
            line = kprintf(line, LIGHT_GREY, "Time: %u:%u:%u",   t.hours, t.minutes, t.seconds);
        }
        else if (startswith(cmd, "random"))
        {
            uint32_t rnd = krand();
            line = kprintf(line, LIGHT_GREY, "Random: %u", rnd);
        }
        else if (startswith(cmd, "help"))
        {
            line = kprintf(line, LIGHT_GREY, "--== COMMAND LIST ==--");
            line = kprintf(line, LIGHT_GREY, "1. help        4. ext2        7. fetch");
            line = kprintf(line, LIGHT_GREY, "2. time        5. test");
            line = kprintf(line, LIGHT_GREY, "3. random      6. fetch");
        }
        else 
        {
            line = kwrite("ERROR: cmd not found", line, RED);
        }
    }

    return 0;
}