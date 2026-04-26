#include "types.h"

static uint32_t next = 19752;

void ksrand(uint32_t seed)
{
    next = seed;
}

uint32_t krand(void)
{
    next = next * 1103515245 + 12345;
    return (next >> 16) & 0x7FFF;
}

uint32_t krand_range(uint32_t min, uint32_t max)
{
    if (max <= min) return min;
    return min + (krand() % (max - min));
}