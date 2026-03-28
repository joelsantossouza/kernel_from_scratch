/*
 * File: fat16.h
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: FAT16, and metadata structures and functions declaration
 */

#ifndef FAT16_H
# define FAT16_H

# include <stdint.h>
# include <stdbool.h>
# include "fs/fat/fat.h"
# include "fs/vfs/vfs_partition.h"

# define IS_FAT16_CLUSTER_FREE(cluster) ({ \
	__typeof__(cluster) _cluster = (cluster); \
	_cluster == 0x0000; \
})

# define IS_FAT16_CLUSTER_RSVD(cluster) ({ \
	__typeof__(cluster) _cluster = (cluster); \
	_cluster == 0x0001 || (_cluster >= 0xFFF0 && _cluster <= 0xFFF6); \
})

# define IS_FAT16_CLUSTER_USED(cluster) ({ \
	__typeof__(cluster) _cluster = (cluster); \
	_cluster >= 0x0002 && _cluster <= 0xFFEF; \
})

# define IS_FAT16_CLUSTER_BAD(cluster) ({ \
	__typeof__(cluster) _cluster = (cluster); \
	_cluster == 0xFFF7; \
})

# define IS_FAT16_CLUSTER_EOC(cluster) ({ \
	__typeof__(cluster) _cluster = (cluster); \
	_cluster >= 0xFFF8; \
})

typedef struct s_phy_fat16_bpb
{
	uint8_t		jmpboot[3];
	uint8_t		oem_name[8];
	uint16_t	bytes_per_sector;
	uint8_t		sectors_per_cluster;
	uint16_t	reserved_sectors_count;
	uint8_t		fats_count;
	uint16_t	root_entry_count;
	uint16_t	total_sectors_16;
	uint8_t		media;
	uint16_t	fat_sectors_16;
	uint16_t	sectors_per_track;
	uint16_t	heads_count;
	uint32_t	hidden_sectors;
	uint32_t	total_sectors_32;
} __attribute__((packed))	t_phy_fat16_bpb;

typedef struct s_phy_fat16_ebpb
{
	uint8_t		drive_number;
	uint8_t		reserved1;
	uint8_t		boot_signature;
	uint32_t	volume_id;
	uint8_t		volume_label[11];
	uint8_t		fs_type[8];
} __attribute__((packed))	t_phy_fat16_ebpb;

typedef struct s_vdl_disk		t_vdl_disk;
typedef struct s_vfs_partition	t_vfs_partition;
typedef struct s_phy_partition	t_phy_partition;

int	fat16_probe(const t_vdl_disk *disk, const t_phy_partition *phy_part, t_fat_metadata *metadata, enum e_fat_errno *fat_err_code);

int	fat16_rootdir_open(const t_fat_dir *rootdir, const char *filename, const char **filename_next, t_fat_file *file);

static inline
enum e_fat_cluster_stat	fat16_cluster_status(uint32_t cluster)
{
	if (IS_FAT16_CLUSTER_USED(cluster) == true)
		return (FAT_CLUSTER_USED);
	if (IS_FAT16_CLUSTER_EOC(cluster) == true)
		return (FAT_CLUSTER_EOC);
	if (IS_FAT16_CLUSTER_FREE(cluster) == true)
		return (FAT_CLUSTER_FREE);
	if (IS_FAT16_CLUSTER_RSVD(cluster) == true)
		return (FAT_CLUSTER_RSVD);
	if (IS_FAT16_CLUSTER_BAD(cluster) == true)
		return (FAT_CLUSTER_BAD);
	return (FAT_CLUSTER_INV);
}

static inline
enum e_fat_cluster_stat	fat16_cluster_next(const t_vfs_partition *part, uint32_t cluster, uint32_t *next)
{
	const t_fat_metadata	*metadata = &part->metadata.fat;

	if (cluster >= metadata->table_entries)
		return (FAT_CLUSTER_INV);
	*next = metadata->table.fat16[cluster];
	return (fat16_cluster_status(*next));
}

#endif
