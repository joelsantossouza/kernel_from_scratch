/*
 * File: vfs_mount_table.c
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: VFS Partition's mounting manipulation APIs and interface functions to
 * 				global mount table
 */

#include <stdint.h>
#include <stddef.h>
#include "fs/vfs/vfs_mount_table.h"
#include "string/string.h"
#include "kernel/config.h"
#include "kernel/macros.h"
#include "errno.h"

static t_vfs_mount	g_vfs_mount_table[VFS_MOUNTS_MAX];
static uint8_t		g_vfs_mount_table_size = 0;

int	vfs_mount_table_insert(const t_vfs_partition *vfs_part, const char *mount_path, uint16_t mount_pathlen)
{
	uint32_t	move_bytes;
	uint8_t		i;

	if (g_vfs_mount_table_size == VFS_MOUNTS_MAX)
		return (-ENOMEM);
	for (i = 0; i < g_vfs_mount_table_size; i++)
		if (mount_pathlen >= g_vfs_mount_table[i].pathlen)
			break ;
	for (; i < g_vfs_mount_table_size; i++)
	{
		if (mount_pathlen > g_vfs_mount_table[i].pathlen)
			break ;
		if (memcmp(mount_path, g_vfs_mount_table[i].path, g_vfs_mount_table[i].pathlen) == 0)
			return (-EBUSY);
	}
	move_bytes = (g_vfs_mount_table_size - i) * sizeof(g_vfs_mount_table[0]);
	memmove(&g_vfs_mount_table[i + 1], &g_vfs_mount_table[i], move_bytes);
	memcpy(g_vfs_mount_table[i].path, mount_path, mount_pathlen);
	g_vfs_mount_table[i].path[mount_pathlen] = 0;
	g_vfs_mount_table[i].pathlen = mount_pathlen;
	g_vfs_mount_table[i].partition = vfs_part;
	g_vfs_mount_table_size++;
	return (KERNEL_SUCCESS);
}

t_vfs_mount	*vfs_mount_table_find(const char *path, uint16_t pathlen)
{
	t_vfs_mount		*mount;
	char			path_next_char;
	uint8_t			i;

	for (i = 0; i < g_vfs_mount_table_size; i++)
		if (pathlen >= g_vfs_mount_table[i].pathlen)
			break ;
	for (; i < g_vfs_mount_table_size; i++)
	{
		mount = &g_vfs_mount_table[i];
		if (UNLIKELY(mount->pathlen == 1))
			return (mount);
		if (memcmp(path, mount->path, mount->pathlen) != 0)
			continue ;
		path_next_char = path[mount->pathlen];
		if (path_next_char != '/' && path_next_char != 0)
			continue ;
		return (mount);
	}
	return (NULL);
}
