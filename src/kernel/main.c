#include "types.h"
#include "kstdio.h"
#include "shell.h"

void k_main()
{
    uint line = 0;
    
    kclear();
    line = kwrite("Welcome to the kernel", line, WHITE);

    shell(line);
}