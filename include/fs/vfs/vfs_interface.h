/*
 * File: vfs_interface.h
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: Polymorphism structures to create compatibility with any File System
 */

#ifndef VFS_INTERFACE_H
# define VFS_INTERFACE_H

# include <stdint.h>
# include "fs/fat/fat16/fat16.h"

typedef union u_vfs_metadata
{
	t_fat16_metadata	fat16;
}	t_vfs_metadata;

typedef int (*t_vfs_probe_fn)(const t_vdl_disk *disk, uint32_t lba, t_vfs_metadata *metadata, int *fs_err_code);

typedef struct s_vfs_interface
{
	t_vfs_probe_fn	probe;
}	t_vfs_interface;

extern const t_vfs_interface	g_fat16_interface;

#endif
