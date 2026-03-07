/*
 * File: vfs_mount.c
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: VFS Partition's mounting manipulation APIs
 */

#include "fs/vfs/config.h"
#include "fs/vfs/partition.h"

static t_partition	g_vfs_mount_table[VFS_MAX_MOUNTS];

int	vfs_mount(const t_disk *disk, uint8_t partition_index, const char *path)
{

}

t_partition	*vfs_mount_find(const char *path)
{

}
