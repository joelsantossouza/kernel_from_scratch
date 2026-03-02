/*
 * File: partition.h
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: VFS partition's Struct implementation
 */

#ifndef VFS_PARTITION_H
# define VFS_PARTITION_H

# include <stdint.h>
# include <stddef.h>
# include "fs/config.h"
# include "kernel/config.h"

typedef struct s_disk		t_disk;
typedef struct s_filesystem	t_filesystem;
struct s_partition
{
	uint8_t				header[FS_MAX_HEADER];
	char				mount_path[KERNEL_MAX_PATH];
	size_t				pathlen;
	size_t				lba_start;
	size_t				lba_end;
	size_t				lba_root;
	const t_disk		*disk;
	const t_filesystem	*fs;
}	t_partition;

#endif
