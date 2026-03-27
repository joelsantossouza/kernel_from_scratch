/*
 * File: fat_dir_open.c
 * Author: Joel Souza
 * Date: 2026-03-25
 * Description: FAT functions to open/close directories
 */

#include <stdbool.h>
#include "fs/fat/fat.h"
#include "kernel/config.h"
#include "errno.h"

int	fat_subdir_open(const t_fat_dir *subdir, const char *filename, const char **filename_next, t_phy_fat_file *entry)
{
	const t_vfs_partition	*part = subdir->partition;
	uint32_t				cluster;
	uint32_t				offset;
	int						bytes_read;

	cluster = subdir->cluster_start;
	offset = 0;
	while (true)
	{
		bytes_read = fat_cluster_read(part, &cluster, &offset, entry, sizeof(*entry));
		if (bytes_read < 0)
			return (bytes_read);
		if (bytes_read == 0 || entry->name[0] == FAT_DIR_ENTRY_UNUSED)
			return (-ENOENT);
		if (entry->name[0] == FAT_DIR_ENTRY_DELETED)
			continue ;
		if (fat_file_match_name(entry, filename, filename_next) == true)
			return (KERNEL_SUCCESS);
	}
}
