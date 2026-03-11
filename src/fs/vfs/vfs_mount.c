/*
 * File: vfs_mount.c
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: VFS Partition's mounting manipulation APIs
 */

#include <stdint.h>
#include "fs/vfs/config.h"
#include "fs/vfs/vfs_partition.h"
#include "drivers/disk/vdl/vdl.h"

static t_vfs_partition	g_vfs_mount_table[VFS_MOUNTS_MAX];

int	vfs_mount(const t_vdl_disk *disk, uint8_t partition_index, const char *path)
{

}

t_vfs_partition	*vfs_mount_find(const char *path)
{

}
