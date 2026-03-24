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
#include "errno.h"

int	fat_cluster_read(const t_vfs_partition *part, uint32_t *cluster, uint32_t *offset, void *buf, uint32_t bytes)
{
	const t_vdl_disk			*disk = part->disk;
	const t_fat_metadata		*metadata = &part->metadata.fat;
	const uint32_t				data_region = metadata->data_region;
	const uint32_t				cluster_bytes = metadata->cluster_bytes;
	const t_fat_cluster_next_fn	fn_cluster_next = metadata->fn_cluster_next;
	uint8_t						*ptr = buf;
	uint32_t					bytes_to_read;
	uint32_t					addr;
	int							exit_stat;

	if (*cluster < FAT_CLUSTER_DATA_START)
		return (-EINVAL);
	if (*offset)
	{
		addr = data_region + (*cluster - FAT_CLUSTER_DATA_START) * cluster_bytes + *offset;
		bytes_to_read = MIN(bytes, cluster_bytes - *offset);
		exit_stat = disk_vdl_read(disk, addr, ptr, bytes_to_read);
		if (exit_stat < 0)
			return (exit_stat);
		ptr += bytes_to_read;
		*offset = (*offset + bytes_to_read) % cluster_bytes;
		if (*offset > 0)
			return (ptr - (uint8_t *)buf);
		exit_stat = fn_cluster_next(part, *cluster, cluster);
		if (exit_stat == FAT_CLUSTER_BAD)
			return (-EIO);
		if (exit_stat != FAT_CLUSTER_USED)
			return (ptr - (uint8_t *)buf);
		bytes -= bytes_to_read;
	}
	while (bytes >= cluster_bytes)
	{
		addr = data_region + (*cluster - FAT_CLUSTER_DATA_START) * cluster_bytes;
		exit_stat = disk_vdl_read(disk, addr, ptr, cluster_bytes);
		if (exit_stat < 0)
			return (exit_stat);
		ptr += cluster_bytes;
		exit_stat = fn_cluster_next(part, *cluster, cluster);
		if (exit_stat == FAT_CLUSTER_BAD)
			return (-EIO);
		if (exit_stat != FAT_CLUSTER_USED)
			return (ptr - (uint8_t *)buf);
		bytes -= cluster_bytes;
	}
	if (bytes > 0)
	{
		addr = data_region + (*cluster - FAT_CLUSTER_DATA_START) * cluster_bytes;
		exit_stat = disk_vdl_read(disk, addr, ptr, bytes);
		if (exit_stat < 0)
			return (exit_stat);
		ptr += bytes;
		*offset = bytes;
	}
	return (ptr - (uint8_t *)buf);
}
