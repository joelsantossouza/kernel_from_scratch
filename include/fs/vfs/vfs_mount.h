/*
 * File: vfs_mount.h
 * Author: Joel Souza
 * Date: 2026-03-16
 * Description: Prototype of interface functions to global mount table
 */

#ifndef VFS_MOUNT_H
# define VFS_MOUNT_H

#include <stdint.h>

typedef struct s_vdl_disk	t_vdl_disk;

int	vfs_mount(const t_vdl_disk *disk, uint8_t partition_idx, const char *mount_path);

#endif
