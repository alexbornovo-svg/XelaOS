#ifndef TIME_H
#define TIME_H

#include "types.h"

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} cmos_time_t;

void cmos_read(cmos_time_t *t);

#endif