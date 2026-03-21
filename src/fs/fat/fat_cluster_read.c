/*
 * File: fat_cluster_read.c
 * Author: Joel Souza
 * Date: 2026-03-20
 * Description: FAT family cluster chain read - Uses the FAT table to read sequential
 * 				clusters disk data into buffer
 */

#include <stdint.h>
#include "fs/vfs/vfs_partition.h"
#include "drivers/disk/vdl/vdl.h"
#include "math/math.h"

int	fat_cluster_read(const t_vfs_partition *part, uint32_t *cluster, uint32_t *offset, void *buf, uint32_t bytes)
{
	const t_vdl_disk			*disk = part->disk;
	const t_fat_metadata		*metadata = &part->metadata.fat;
	const uint32_t				cluster_start = metadata->cluster_start;
	const uint32_t				cluster_bytes = metadata->cluster_bytes;
	const t_fat_cluster_next_fn	fat_cluster_next = metadata->fat_cluster_next;
	uint8_t						*ptr = buf;
	uint32_t					bytes_to_read;
	uint32_t					addr;
	int							err_code;

	if (*offset)
	{
		addr = cluster_start + *cluster * cluster_bytes + *offset;
		bytes_to_read = MIN(bytes, cluster_bytes - *offset);
		err_code = disk_vdl_read(disk, addr, ptr, bytes_to_read);
		if (err_code < 0)
			return (err_code);
		ptr += bytes_to_read;
		bytes -= bytes_to_read;
		*offset = (*offset + bytes_to_read) % cluster_bytes;
		if (*offset > 0)
			return (ptr - (uint8_t *)buf);
		if (fat_cluster_next(part, *cluster, cluster) == SOMETHING)
			return (ptr - (uint8_t *)buf);
	}
	while (bytes >= cluster_bytes)
	{
		addr = cluster_start + *cluster * cluster_bytes + *offset;
		err_code = disk_vdl_read(disk, addr, ptr, cluster_bytes);
		if (err_code < 0)
			return (err_code);
		ptr += cluster_bytes;
		bytes -= cluster_bytes;
		if (fat_cluster_next(part, *cluster, cluster) == SOMETHING)
			return (ptr - (uint8_t *)buf);
	}
	if (bytes > 0)
	{
		addr = cluster_start + *cluster * cluster_bytes + *offset;
		err_code = disk_vdl_read(disk, addr, ptr, bytes);
		if (err_code < 0)
			return (err_code);
		*offset = bytes;
	}
	return (ptr - (uint8_t *)buf);
}
