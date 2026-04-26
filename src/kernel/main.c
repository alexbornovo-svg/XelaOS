#include "types.h"
#include "kstdio.h"
#include "shell.h"
#include "random.h"
#include "ata_pio.h"
#include "time.h"
#include "gdt.h"
#include "idt.h"

void k_main()
{
    uint line = 0;
    gdt_init();
    kclear();
    idt_init(); // NON FUNZIONA PER ORA

    cmos_time_t t;
    cmos_read(&t);
    ksrand(t.seconds + t.minutes * 60 + t.hours * 3600);

    line = kwrite("Searching for disk", line, WHITE);
    line = ext2_init(line);

    line++;
    line = kwrite("Welcome to the kernel", line, WHITE);
    shell(line);
}