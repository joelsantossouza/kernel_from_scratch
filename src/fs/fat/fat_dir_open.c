/*
 * File: fat_dir_open.c
 * Author: Joel Souza
 * Date: 2026-03-25
 * Description: FAT functions to init/open/close directories
 */

#include <stdbool.h>
#include "fs/fat/fat.h"
#include "fs/vfs/vfs_partition.h"
#include "kernel/config.h"
#include "errno.h"

int	fat_rootdir_init(const t_vfs_partition *part, t_fat_dir *rootdir)
{
	const t_fat_metadata	*metadata = &part->metadata.fat;

	rootdir->partition = part;
	rootdir->entry = (t_phy_fat_file){0};
	rootdir->start = metadata->rootdir;
	rootdir->bytes = metadata->data_region - metadata->rootdir;
	return (KERNEL_SUCCESS);
}

int	fat_dir_open(const t_fat_dir *dir, const char *filename, const char **filename_next, t_fat_file *file)
{
	const t_vfs_partition	*part = dir->partition;
	t_phy_fat_file			entry;
	uint32_t				cluster;
	uint32_t				offset;
	int						bytes_read;

	cluster = dir->start;
	offset = 0;
	while (true)
	{
		bytes_read = fat_cluster_read(part, &cluster, &offset, &entry, sizeof(entry));
		if (bytes_read < 0)
			return (bytes_read);
		if (bytes_read == 0 || entry.name[0] == FAT_DIR_ENTRY_UNUSED)
			return (-ENOENT);
		if (entry.name[0] == FAT_DIR_ENTRY_DELETED)
			continue ;
		if (entry.name[0] == '.')
			continue ;
		if (fat_file_match_name(&entry, filename, filename_next) == false)
			continue ;
		fat_file_init(part, &entry, file);
		return (KERNEL_SUCCESS);
	}
}
