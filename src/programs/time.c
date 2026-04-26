#include "types.h"
#include "time.h"
#include "kstdio.h"

static uint8_t cmos_read_reg(uint8_t reg)
{
    __asm__ volatile ("outb %0, %1" : : "a"((uint8_t)reg), "Nd"((uint16_t)0x70));
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"((uint16_t)0x71));
    return val;
}

static uint8_t bcd_to_bin(uint8_t bcd)
{
    return (bcd & 0x0F) + ((bcd >> 4) * 10);
}

static void cmos_wait_ready(void)
{
    while (cmos_read_reg(0x0A) & 0x80);
}

void cmos_read(cmos_time_t *t)
{
    cmos_wait_ready();

    t->seconds = bcd_to_bin(cmos_read_reg(0x00));
    t->minutes = bcd_to_bin(cmos_read_reg(0x02));
    t->hours = bcd_to_bin(cmos_read_reg(0x04));
    t->day = bcd_to_bin(cmos_read_reg(0x07));
    t->month = bcd_to_bin(cmos_read_reg(0x08));
    t->year = bcd_to_bin(cmos_read_reg(0x09));
}