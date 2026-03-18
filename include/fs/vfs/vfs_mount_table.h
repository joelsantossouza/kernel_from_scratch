/*
 * File: vfs_mount_table.h
 * Author: Joel Souza
 * Date: 2026-03-16
 * Description: Prototype of interface functions to global mount table
 */

#ifndef VFS_MOUNT_TABLE_H
# define VFS_MOUNT_TABLE_H

# include <stdint.h>
# include "fs/vfs/config.h"

typedef struct s_vdl_disk		t_vdl_disk;
typedef struct s_vfs_partition	t_vfs_partition;

typedef struct s_vfs_mount
{
	char					path[VFS_PATH_MAX + 1];
	const t_vfs_partition	*partition;
	uint16_t				pathlen;
}	t_vfs_mount;

#endif
