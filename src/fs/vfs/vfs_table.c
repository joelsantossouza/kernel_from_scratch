/*
 * File: vfs_table.c
 * Author: Joel Souza
 * Date: 2026-03-12
 * Description: Table of all filesystem interfaces supported by the system
 */

#include "fs/vfs/vfs_interface.h"
#include "fs/vfs/config.h"

// Hashed by MBR partition type code (see boot/mbr.h)
static const t_vfs_interface	*g_vfs_table_hash[VFS_TABLE_HASH_MAX] = {0};

static const t_vfs_interface	*g_vfs_table[VFS_TABLE_MAX] = {0};
static uint8_t					g_vfs_table_size = ;

int	vfs_table_detect_fs(const t_phy_partition *phy_part, const t_vfs_interface **fs)
{

}
