/*
 * File: vfs.h
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: Polymorphism structures to create compatibility with any File System
 */

#ifndef VFS_H
# define VFS_H

# include <stddef.h>
# include "fs/fat16/fat16.h"
# include "kernel/config.h"

typedef struct s_disk	t_disk;

typedef union u_vfs_metadata
{
	t_fat16_metadata	fat16;
}	t_vfs_metadata;

typedef struct s_vfs_partition
{
	t_vfs_metadata	vfs_metadata;
	char			mount_path[KERNEL_MAX_PATH];
	size_t			pathlen;
	const t_disk	*disk;
	const t_vfs		*vfs;
}	t_vfs_partition;

#endif
