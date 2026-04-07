/*
 * File: vfs_partition.c
 * Author: Joel Souza
 * Date: 2026-03-11
 * Description: Virtual and Physical Partitions initialization and management functions
 */

#include "fs/vfs/vfs_partition.h"
#include "fs/vfs/vfs_table.h"
#include "drivers/disk/vdl/vdl.h"
#include "kernel/kernel.h"
#include "boot/mbr.h"

int	vfs_partition_init(const t_vdl_disk *disk, uint8_t part_idx, t_vfs_partition *vfs_part)
{
	t_phy_partition	phy_part;
	int				err_code;

	err_code = vdl_read(disk, MBR_PART_TABLE(part_idx), &phy_part, sizeof(phy_part));
	if (err_code != KERNEL_SUCCESS)
		return (err_code);
	err_code = vfs_table_detect_fs(disk, &phy_part, &vfs_part->vfs, &vfs_part->metadata);
	if (err_code != KERNEL_SUCCESS)
		return (err_code);
	vfs_part->disk = disk;
	return (KERNEL_SUCCESS);
}
