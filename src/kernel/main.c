#include "types.h"
#include "kstdio.h"
#include "shell.h"
#include "ata_pio.h"

void k_main()
{
    uint line = 0;
    kclear();

    line = kwrite("Searching for disk", line, WHITE);
    line = ext2_init(line);
    line++;
    line = kwrite("Welcome to the kernel", line, WHITE);

    shell(line);
}