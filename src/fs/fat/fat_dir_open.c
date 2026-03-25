/*
 * File: fat_dir_open.c
 * Author: Joel Souza
 * Date: 2026-03-25
 * Description: FAT functions to open/close directories
 */

#include <stdbool.h>
#include "fs/fat/fat.h"
#include "kernel/config.h"

int	fat_subdir_open(const t_vfs_dir *subdir, const char *filename, const char **filename_next, t_phy_fat_file *file)
{
	const t_vfs_partition	*part = subdir->partition;
	uint32_t				cluster;
	uint32_t				offset;
	int						err_code;

	cluster = (subdir->cluster_high16bits << 16) | subdir->cluster_low16bits;
	offset = 0;
	while (true)
	{
		err_code = fat_cluster_read(part, &cluster, &offset, file, sizeof(*file));
		if (err_code != KERNEL_SUCCESS)
			return (err_code);
		if (file->name[0] == FAT_DIR_ENTRY_UNUSED)
			return (NOT_FOUND);
		if (file->name[0] == FAT_DIR_ENTRY_DELETED)
			continue ;
		if (fat_file_cmp_name(file, filename, filename_next) == 0)
			return (KERNEL_SUCCESS);
	}
}
