/*
 * File: vfs_mount.c
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: VFS Partition's mounting manipulation APIs and interface functions to
 * 				global mount table
 */

#include <stdint.h>
#include "fs/vfs/config.h"
#include "fs/vfs/vfs_partition.h"
#include "drivers/disk/vdl/vdl.h"
#include "string/string.h"
#include "kernel/config.h"
#include "errno.h"

static t_vfs_partition	g_vfs_mount_table[VFS_MOUNTS_MAX];
static uint8_t			g_vfs_mount_table_size = 0;

int	vfs_mount(const t_vdl_disk *disk, uint8_t partition_idx, const char *mount_path)
{
	t_vfs_partition	vfs_part;
	uint32_t		mount_pathlen;
	uint32_t		move_bytes;
	uint8_t			i;
	int				init_stat;

	if (g_vfs_mount_table_size == VFS_MOUNTS_MAX)
		return (-ENOMEM);
	if (strnlen_strict(mount_path, VFS_PATH_MAX, &mount_pathlen) != KERNEL_SUCCESS)
		return (-ENAMETOOLONG);
	for (i = 0; i < g_vfs_mount_table_size; i++)
		if (mount_pathlen >= g_vfs_mount_table[i].mount_pathlen)
			break ;
	for (; i < g_vfs_mount_table_size; i++)
	{
		if (mount_pathlen > g_vfs_mount_table[i].mount_pathlen)
			break ;
		if (strcmp(mount_path, g_vfs_mount_table[i].mount_path) == 0)
			return (-EBUSY);
	}
	init_stat = vfs_partition_init(&vfs_part, disk, partition_idx, mount_path);
	if (init_stat != KERNEL_SUCCESS)
		return (init_stat);
	move_bytes = (g_vfs_mount_table_size - i) * sizeof(g_vfs_mount_table[0]);
	memmove(&g_vfs_mount_table[i + 1], &g_vfs_mount_table[i], move_bytes);
	g_vfs_mount_table[i] = vfs_part;
	g_vfs_mount_table_size++;
	return (KERNEL_SUCCESS);
}

t_vfs_partition	*vfs_mount_find(const char *path)
{
	(void)path; // WARNING: NOT IMPLEMENTED YET
	return (0);
}
