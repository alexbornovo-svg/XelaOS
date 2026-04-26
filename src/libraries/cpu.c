#include "types.h"

void cpu_brand(char *buf)
{
    unsigned int eax, ebx, ecx, edx;
    int i, j = 0;

    unsigned int regs[3][4];
    for (i = 0; i < 3; i++)
    {
        eax = 0x80000002 + i;
        __asm__ volatile (
            "cpuid"
            : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
            : "a"(eax)
        );
        regs[i][0] = eax;
        regs[i][1] = ebx;
        regs[i][2] = ecx;
        regs[i][3] = edx;
    }

    for (i = 0; i < 3; i++)
    {
        int k;
        for (k = 0; k < 4; k++)
        {
            unsigned int r = regs[i][k];
            buf[j++] = (r      ) & 0xFF;
            buf[j++] = (r >>  8) & 0xFF;
            buf[j++] = (r >> 16) & 0xFF;
            buf[j++] = (r >> 24) & 0xFF;
        }
    }
    buf[j] = '\0';
}