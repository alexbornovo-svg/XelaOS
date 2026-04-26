#ifndef ATA_PIO_H
#define ATA_PIO_H

#include "types.h"

int ata_read_sector(uint8_t bus_offset, uint8_t drive, uint32_t lba, uint8_t *buf);
void ata_reset();

uint16_t inw(unsigned short port);
uint ext2_init(uint line);

extern ext2_superblock_t *global_sb;

#endif