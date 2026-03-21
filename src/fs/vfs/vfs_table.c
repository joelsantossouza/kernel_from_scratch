/*
 * File: vfs_table.c
 * Author: Joel Souza
 * Date: 2026-03-12
 * Description: Table of all filesystem interfaces supported by the system
 */

#include <stddef.h>
#include "fs/vfs/vfs_interface.h"
#include "fs/fat/fat16/fat16.h"
#include "fs/vfs/vfs_partition.h"
#include "boot/mbr.h"
#include "fs/vfs/config.h"
#include "kernel/config.h"
#include "kernel/macros.h"
#include "errno.h"

// Hashed by MBR partition type code (see boot/mbr.h)
static const t_vfs_interface	*g_vfs_table_hash[VFS_TABLE_HASH_MAX] = {
	[MBR_PART_TYPE_FAT16_LBA] = &g_fat16_interface,
};

static inline
const t_vfs_interface	*vfs_table_hash_get(uint8_t	partition_type_code)
{
	return (g_vfs_table_hash[partition_type_code]);
}

// Linear table with all supported filesystems
static const t_vfs_interface	*g_vfs_table[] = {
	&g_fat16_interface,
};
static uint16_t					g_vfs_table_size = ARRAY_SIZE(g_vfs_table);

int	vfs_table_detect_fs(const t_vdl_disk *disk, const t_phy_partition *phy_part, const t_vfs_interface **fs, t_vfs_metadata *metadata)
{
	const t_vfs_interface	*fs_hint = vfs_table_hash_get(phy_part->type_code);
	uint16_t				i;
	int						exit_stat;

	if (fs_hint != NULL)
	{
		exit_stat = fs_hint->probe(disk, phy_part, metadata, NULL);
		if (exit_stat != -EBADFS)
		{
			*fs = exit_stat == KERNEL_SUCCESS ? fs_hint : NULL;
			return (exit_stat);
		}
	}
	for (i = 0; i < g_vfs_table_size; i++)
	{
		if (UNLIKELY(g_vfs_table[i] == fs_hint))
			continue ;
		exit_stat = g_vfs_table[i]->probe(disk, phy_part, metadata, NULL);
		if (exit_stat == -EBADFS)
			continue ;
		*fs = exit_stat == KERNEL_SUCCESS ? g_vfs_table[i] : NULL;
		return (exit_stat);
	}
	return (-EFSNOSUPPORT);
}
