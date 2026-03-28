/*
 * File: vfs_partition.h
 * Author: Joel Souza
 * Date: 2026-03-11
 * Description: Virtual and Physical partition structures representation
 */

#ifndef VFS_PARTITION_H
# define VFS_PARTITION_H

# include <stdint.h>
# include "fs/fat/fat.h"

typedef struct s_phy_partition
{
	uint8_t		boot_flag;
	uint8_t		chs_start[3];
	uint8_t		type_code;
	uint8_t		chs_end[3];
	uint32_t	lba_start;
	uint32_t	total_sectors;
} __attribute__((packed))	t_phy_partition;

typedef union u_vfs_metadata
{
	t_fat_metadata	fat;
}	t_vfs_metadata;

typedef struct s_vdl_disk		t_vdl_disk;
typedef struct s_vfs_interface	t_vfs_interface;

typedef struct s_vfs_partition
{
	t_vfs_metadata			metadata;
	const t_vdl_disk		*disk;
	const t_vfs_interface	*vfs;
}	t_vfs_partition;

#endif
