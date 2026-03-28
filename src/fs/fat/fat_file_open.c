/*
 * File: fat_file_open.c
 * Author: Joel Souza
 * Date: 2026-03-28
 * Description: FAT functions to init/open/close files
 */

#include "fs/vfs/vfs_partition.h"
#include "fs/fat/fat.h"
#include "kernel/config.h"

int	fat_file_init(const t_vfs_partition *part, const t_phy_fat_file *entry, t_fat_file *file)
{
	file->partition = part;
	file->entry = *entry;
	file->start = (entry->cluster_high16bits << 16) | entry->cluster_low16bits;
	file->bytes = entry->bytes;
	return (KERNEL_SUCCESS);
}

int	fat_file_open(const t_vfs_partition *part, const char *abspath, t_fat_file *file)
{
	const t_fat_metadata	*metadata = &part->metadata.fat;
	t_fat_file				rootdir;
	int						err_code;

	fat_rootdir_init(part, &rootdir);
	err_code = metadata->fn_rootdir_open(&rootdir, abspath, &abspath, file);
	if (err_code != KERNEL_SUCCESS)
		return (err_code);
	while (*abspath != 0)
	{
		err_code = fat_dir_open(file, abspath, &abspath, file);
		if (err_code != KERNEL_SUCCESS)
			return (err_code);
	}
	return (KERNEL_SUCCESS);
}
