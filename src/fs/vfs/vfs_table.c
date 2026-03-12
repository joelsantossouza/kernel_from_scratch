/*
 * File: vfs_table.c
 * Author: Joel Souza
 * Date: 2026-03-12
 * Description: Table of all filesystem interfaces supported by the system
 */

#include "fs/vfs/vfs_interface.h"
#include "fs/vfs/vfs_partition.h"
#include "boot/mbr.h"
#include "fs/vfs/config.h"

// Hashed by MBR partition type code (see boot/mbr.h)
static const t_vfs_interface	*g_vfs_table_hash[VFS_TABLE_HASH_MAX] = {
	[MBR_PART_TYPE_FAT16_LBA] = &g_fat16_interface,
};

static const t_vfs_interface	*g_vfs_table[VFS_TABLE_MAX] = {
	&g_fat16_interface,
};
static uint8_t					g_vfs_table_size = ;

int	vfs_table_detect_fs(const t_phy_partition *phy_part, const t_vfs_interface **fs, t_vfs_metadata *metadata)
{
	const t_vfs_interface	*fs_hint = vfs_table_hash_get(phy_part->type_code);
	uint8_t					i;
	int						exit_stat;

	if (fs_hint != NULL)
	{
		exit_stat = fs_hint->probe(phy_part->lba_start, metadata);
		if (exit_stat != INVALID_FILESYSTEM)
		{
			*fs = exit_stat == KERNEL_SUCCESS ? fs_hint : NULL;
			return (exit_stat);
		}
	}
	for (i = 0; i < g_vfs_table_size; i++)
	{
		if (unlikely(g_vfs_table[i] == fs_hint))
			continue ;
		exit_stat = g_vfs_table[i]->probe(phy_part->lba_start, metadata);
		if (exit_stat == INVALID_FILESYSTEM)
			continue ;
		*fs = exit_stat == KERNEL_SUCCESS ? g_vfs_table[i] : NULL;
		return (exit_stat);
	}
	return (FILESYSTEM_NOT_SUPPORTED);
}
