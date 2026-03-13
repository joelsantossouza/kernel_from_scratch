/*
 * File: vfs_table.h
 * Author: Joel Souza
 * Date: 2026-03-13
 * Description: Routine declarations to interact with the global
 * 				Virtual File Systems Table
 */

#ifndef VFS_TABLE_H
# define VFS_TABLE_H

typedef struct s_vdl_disk		t_vdl_disk;
typedef struct s_phy_partition	t_phy_partition;
typedef struct s_vfs_interface	t_vfs_interface;
typedef struct s_vfs_metadata	t_vfs_metadata;

int	vfs_table_detect_fs(const t_vdl_disk *disk, const t_phy_partition *phy_part, const t_vfs_interface **fs, t_vfs_metadata *metadata);

#endif
