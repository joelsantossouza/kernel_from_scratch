/*
 * File: vfs_partition.c
 * Author: Joel Souza
 * Date: 2026-03-11
 * Description: Virtual Partitions initialization and management functions
 */

#include "fs/vfs/vfs_partition.h"
#include "fs/vfs/vfs_table.h"
#include "drivers/disk/vdl/vdl.h"
#include "string/string.h"
#include "kernel/config.h"
#include "boot/mbr.h"
#include "errno.h"

int	vfs_partition_init(t_vfs_partition *vfs_part, const t_vdl_disk *disk, uint8_t part_idx, const char *mount_path)
{
	t_phy_partition	phy_part;
	uint32_t		mount_pathlen;
	int				err_code;

	err_code = disk_vdl_read(disk, MBR_PART_TABLE(part_idx), &phy_part, sizeof(phy_part));
	if (err_code != KERNEL_SUCCESS)
		return (err_code);
	err_code = vfs_table_detect_fs(disk, &phy_part, &vfs_part->vfs, &vfs_part->metadata);
	if (err_code != KERNEL_SUCCESS)
		return (err_code);
	if (strnlen_strict(mount_path, VFS_PATH_MAX, &mount_pathlen) != KERNEL_SUCCESS)
		return (-ENAMETOOLONG);
	memcpy(vfs_part->mount_path, mount_path, mount_pathlen + 1);
	vfs_part->mount_pathlen = mount_pathlen;
	vfs_part->disk = disk;
	return (KERNEL_SUCCESS);
}
