#ifndef TYPES_H
#define TYPES_H

typedef unsigned char  uchar;
typedef unsigned int   uint;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

typedef struct {
    uint32_t inodes_count;          // 0
    uint32_t blocks_count;          // 4
    uint32_t r_blocks_count;        // 8
    uint32_t free_blocks_count;     // 12
    uint32_t free_inodes_count;     // 16
    uint32_t first_data_block;      // 20
    uint32_t log_block_size;        // 24
    uint32_t log_frag_size;         // 28
    uint32_t blocks_per_group;      // 32
    uint32_t frags_per_group;       // 36
    uint32_t inodes_per_group;      // 40
    uint32_t mtime;                 // 44
    uint32_t wtime;                 // 48
    uint16_t mnt_count;             // 52
    uint16_t max_mnt_count;         // 54
    uint16_t magic;                 // 56
    uint16_t state;                 // 58
    uint16_t errors;                // 60
    uint16_t minor_rev_level;       // 62
    uint32_t lastcheck;             // 64
    uint32_t checkinterval;         // 68
    uint32_t creator_os;            // 72
    uint32_t rev_level;             // 76
    uint16_t def_resuid;            // 80
    uint16_t def_resgid;            // 82

    uint32_t first_ino;
    uint16_t inode_size;
    uint16_t block_group_nr;
    uint32_t feature_compat;
    uint32_t feature_incompat;
    uint32_t feature_ro_compat;
    uint8_t  uuid[16];
    char     volume_name[16];
    char     last_mounted[64];
    uint32_t algo_bitmap;

    uint8_t  prealloc_blocks;
    uint8_t  prealloc_dir_blocks;
    uint16_t _unused_padding;

    uint8_t  journal_uuid[16];
    uint32_t journal_inum;
    uint32_t journal_dev;
    uint32_t last_orphan;

    uint32_t hash_seed[4];
    uint8_t  def_hash_version;
    uint8_t  _reserved_char[3];

    // -- Altre opzioni --
    uint32_t default_mount_options;
    uint32_t first_meta_bg;

    uint8_t  _unused[760];
} __attribute__((packed)) ext2_superblock_t;

typedef struct {
    uint16_t mode;
    uint32_t size;
    uint32_t block[15];
} __attribute__((packed)) ext2_inode_t;

typedef struct {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t  name_len;
    uint8_t  file_type;
    char     name[];
} __attribute__((packed)) ext2_dirent_t;

#endif