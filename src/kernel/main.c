#include "types.h"
#include "kstdio.h"
#include "shell.h"
#include "random.h"
#include "ata_pio.h"
#include "time.h"
#include "idt.h"

void k_main()
{
    uint line = 0;
    kclear();
    idt_init();

    cmos_time_t t;
    cmos_read(&t);
    ksrand(t.seconds + t.minutes * 60 + t.hours * 3600);

    line = kwrite("Searching for disk", line, WHITE);
    line = ext2_init(line);

    __asm__ volatile ("sti");

    line++;
    line = kwrite("Welcome to the kernel", line, WHITE);
    shell(line);
}