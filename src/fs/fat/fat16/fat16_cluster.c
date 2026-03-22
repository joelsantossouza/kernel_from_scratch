/*
 * File: fat16_cluster.c
 * Author: Joel Souza
 * Date: 2026-03-21
 * Description: Functions to resolve clusters index with FAT Table lookups
 */

#include <stdbool.h>
#include "fs/vfs/vfs_partition.h"
#include "fs/fat/fat.h"
#include "fs/fat/fat16/fat16.h"

enum e_fat_cluster_stat	fat16_cluster_next(const t_vfs_partition *part, uint32_t cluster, uint32_t *next)
{
	const t_fat_metadata	*metadata = &part->metadata.fat;

	if (cluster >= metadata->table_entries)
		return (FAT_CLUSTER_INV);
	*next = metadata->table.fat16[cluster];
	if (IS_FAT16_CLUSTER_FREE(*next) == true)
		return (FAT_CLUSTER_FREE);
	if (IS_FAT16_CLUSTER_RSVD(*next) == true)
		return (FAT_CLUSTER_RSVD);
	if (IS_FAT16_CLUSTER_EOC(*next) == true)
		return (FAT_CLUSTER_EOC);
	if (IS_FAT16_CLUSTER_BAD(*next) == true)
		return (FAT_CLUSTER_BAD);
	return (FAT_CLUSTER_USED);
}
