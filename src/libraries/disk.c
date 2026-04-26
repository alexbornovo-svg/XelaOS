#include "types.h"

void get_disk_usage(ext2_superblock_t *sb, uint32_t *free_mb, uint32_t *total_mb) {
    uint32_t block_size_kb = (1024 << sb->log_block_size) / 1024;

    *free_mb = (sb->free_blocks_count * block_size_kb) / 1024;
    *total_mb = (sb->blocks_count * block_size_kb) / 1024;
}