#ifndef RANDOM_H
#define RANDOM_H

#include "types.h"

void ksrand(uint32_t seed);
uint32_t krand(void);
uint32_t krand_range(uint32_t min, uint32_t max);

#endif