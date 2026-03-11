/*
 * File: vfs_partition.h
 * Author: Joel Souza
 * Date: 2026-03-11
 * Description: Virtual and Physical partition structures representation
 */

#ifndef VFS_PARTITION_H
# define VFS_PARTITION_H

# include <stdint.h>
# include "fs/vfs/config.h"

typedef struct s_phy_partition
{

}	t_phy_partition;

typedef struct s_vdl_disk		t_vdl_disk;
typedef struct s_vfs_interface	t_vfs_interface;

typedef struct s_vfs_partition
{
	t_vfs_metadata			metadata;
	char					mount_path[VFS_PATH_MAX + 1];
	const t_vdl_disk		*disk;
	const t_vfs_interface	*vfs;
	uint16_t				mount_pathlen;
}	t_vfs_partition;


#endif
