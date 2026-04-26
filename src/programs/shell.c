#include "types.h"
#include "kstdio.h"

int shell(int line)
{
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