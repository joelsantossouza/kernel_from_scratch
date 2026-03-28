/*
 * File: fat16_dir_open.c
 * Author: Joel Souza
 * Date: 2026-03-28
 * Description: FAT16's specific functions to open/close directories
 */

#include "fs/fat/fat.h"
#include "fs/vfs/vfs_partition.h"
#include "drivers/disk/vdl/vdl.h"
#include "kernel/config.h"
#include "errno.h"

int	fat16_rootdir_open(const t_fat_dir *rootdir, const char *filename, const char **filename_next, t_fat_file *file)
{
	const t_vdl_disk	*disk = rootdir->partition->disk;
	const uint32_t		rootdir_bytes = rootdir->bytes;
	const uint32_t		rootdir_addr = rootdir->start;
	t_phy_fat_file		entry;
	uint32_t			offset;
	int					err_code;

	for (offset = 0; offset < rootdir_bytes; offset += sizeof(t_phy_fat_file))
	{
		err_code = disk_vdl_read(disk, rootdir_addr + offset, &entry, sizeof(entry));
		if (err_code != KERNEL_SUCCESS)
			return (err_code);
		if (entry.name[0] == FAT_DIR_ENTRY_UNUSED)
			return (-ENOENT);
		if (entry.name[0] == FAT_DIR_ENTRY_DELETED)
			continue ;
		if (fat_file_match_name(&entry, filename, filename_next) == false)
			continue ;
		fat_file_init(rootdir->partition, &entry, file);
		return (KERNEL_SUCCESS);
	}
	return (-ENOENT);
}
