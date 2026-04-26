#include "types.h"
#include "kbd.h"
#include "kstdio.h"
#include "ata_pio.h"

ext2_superblock_t *global_sb = 0;
static ext2_superblock_t sb_buf;

uint16_t inw(unsigned short port)
{
    uint16_t result;
    __asm__ volatile ("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void ata_reset(void)
{
    outb(0x3F6, 0x04);
    outb(0x3F6, 0x00);
    uint32_t i;
    for (i = 0; i < 100000; i++)
    {
        inb(0x1F7);
    }
}

int ata_read_sector(uint8_t bus_offset, uint8_t drive, uint32_t lba, uint8_t *buf) {
    uint16_t port = (bus_offset == 0) ? 0x1F0 : 0x170;
    
    uint8_t status = inb(port + 7);
    if (status == 0xFF) return -1;

    uint32_t timeout = 100000;
    while ((inb(port + 7) & 0x80) && --timeout);
    if (timeout == 0) return -2;

    outb(port + 6, (0xE0 | (drive << 4)) | ((lba >> 24) & 0x0F));
    
    for(int i=0; i<5; i++) inb(port + 7);

    outb(port + 2, 1);
    outb(port + 3, (uint8_t)lba);
    outb(port + 4, (uint8_t)(lba >> 8));
    outb(port + 5, (uint8_t)(lba >> 16));
    outb(port + 7, 0x20);

    timeout = 100000;
    while (!(inb(port + 7) & 0x08) && --timeout);
    if (timeout == 0) return -3;

    for (int i = 0; i < 256; i++) {
        ((uint16_t*)buf)[i] = inw(port);
    }

    return 0;
}



uint ext2_init(uint line) {
    int res;
    uint8_t sector_a[512], sector_b[512];

    kwrite("Searching Primary Master...", line++, WHITE);
    res  = ata_read_sector(0, 0, 2, sector_a);
    res |= ata_read_sector(0, 0, 3, sector_b);

    if (res == 0) 
    {
        kmemcpy((uint8_t*)&sb_buf,       sector_a, 512);
        kmemcpy((uint8_t*)&sb_buf + 512, sector_b, 512);

        if (sb_buf.magic == 0xEF53) 
        {
            global_sb = &sb_buf;
            return kwrite("Disk finded on Primary Master!", line, GREEN);
        }
    }

    kwrite("Searching Primary Slave...", line++, WHITE);
    res  = ata_read_sector(0, 1, 2, sector_a);
    res |= ata_read_sector(0, 1, 3, sector_b);

    if (res == 0) 
    {
        kmemcpy((uint8_t*)&sb_buf,       sector_a, 512);
        kmemcpy((uint8_t*)&sb_buf + 512, sector_b, 512);

        if (sb_buf.magic == 0xEF53) 
        {
            global_sb = &sb_buf;
            return kwrite("Disk finded on Primary Slave!", line, GREEN);
        }
    }

    kwrite("Searching Secondary Master...", line++, WHITE);
    res  = ata_read_sector(1, 0, 2, sector_a);
    res |= ata_read_sector(1, 0, 3, sector_b);
    
    if (res == 0) 
    {
        kmemcpy((uint8_t*)&sb_buf,       sector_a, 512);
        kmemcpy((uint8_t*)&sb_buf + 512, sector_b, 512);

        if (sb_buf.magic == 0xEF53) 
        {
            global_sb = &sb_buf;
            return kwrite("Disk finded on Secondary Master!", line, GREEN);
        }
    }
    return kwrite("ERROR: No valid EXT2 disk.", line, RED);
}