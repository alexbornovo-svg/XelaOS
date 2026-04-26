#ifndef DISK_H
#define DISK_H

#include "types.h"

void get_disk_usage(ext2_superblock_t *sb, uint32_t *free_mb, uint32_t *total_mb);

#endif