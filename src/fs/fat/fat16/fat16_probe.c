/*
 * File: fat16_probe.c
 * Author: Joel Souza
 * Date: 2026-03-12
 * Description: FAT16 detection and validation
 */

#include "kernel/config.h"
#include "fs/fat/fat.h"
#include "fs/fat/fat16/fat16.h"
#include "fs/vfs/vfs_partition.h"
#include "drivers/disk/disk.h"
#include "drivers/disk/vdl/vdl.h"
#include "kernel/macros.h"
#include "math/math.h"
#include "errno.h"

static inline
int	fat16_probe_bpb(const t_phy_fat16_bpb *bpb)
{
	if ((bpb->jmpboot[0] != FAT_JMP_OPCODE_SHORT || bpb->jmpboot[2] != FAT_NOP_OPCODE)
		&& bpb->jmpboot[0] != FAT_JMP_OPCODE_NEAR)
		return (FAT_EBAD_JMPBOOT);
	if (bpb->bytes_per_sector != 512 && bpb->bytes_per_sector != 1024
		&& bpb->bytes_per_sector != 2048 && bpb->bytes_per_sector != 4096)
		return (FAT_EBAD_BYTES_PER_SECT);
	if (bpb->sectors_per_cluster == 0 || !IS_POWER_2(bpb->sectors_per_cluster))
		return (FAT_EBAD_SECTS_PER_CLUS);
	if (bpb->reserved_sectors_count == 0)
		return (FAT_EBAD_RSVD_SECTS_CNT);
	if (bpb->fats_count == 0)
		return (FAT_EBAD_FATS_CNT);
	if (bpb->root_entry_count == 0)
		return (FAT16_EBAD_ROOT_ENTS_CNT);
	if (bpb->total_sectors_16 == 0 && bpb->total_sectors_32 == 0)
		return (FAT_EBAD_TOTAL_SECTS);
	if (bpb->media != FAT_MEDIA_FIXED && bpb->media != FAT_MEDIA_REMOVABLE)
		return (FAT_EBAD_MEDIA);
	if (bpb->fat_sectors_16 == 0)
		return (FAT16_EBAD_FAT_SECTS);
	return (KERNEL_SUCCESS);
}

static
int	fat16_metadata_init(const t_vdl_disk *disk, const t_phy_partition *phy_part, const t_phy_fat16_bpb *bpb, t_fat_metadata *metadata)
{
	const uint32_t	partition_addr = SECTORS_TO_BYTES(phy_part->lba_start);
	const uint32_t	sector_bytes = bpb->bytes_per_sector;
	const uint32_t	cluster_bytes = sector_bytes * bpb->sectors_per_cluster;
	const uint32_t	fat_table_addr = partition_addr + bpb->reserved_sectors_count * sector_bytes;
	const uint32_t	fat_table_bytes = bpb->fat_sectors_16 * sector_bytes;
	int				err_code;

	err_code = vdl_read(disk, fat_table_addr, metadata->table.fat16, 0xff);
	if (err_code != KERNEL_SUCCESS)
		return (err_code);
	metadata->table_entries = 0xff;
	// metadata->fat_table = kmalloc(); WARNING: Future implementation with malloc
	// ...
	// err_code = disk_vdl_read(disk, fat_table_addr, metadata->table.fat16, fat_table_bytes);
	// if (err_code != KERNEL_SUCCESS)
	// 	return (err_code);
	// metadata->table_entries = fat_table_bytes / 16  WARNING: Future implementation with malloc
	metadata->rootdir = fat_table_addr + bpb->fats_count * fat_table_bytes;
	metadata->data_region = metadata->rootdir + bpb->root_entry_count * sizeof(t_phy_fat_file);
	metadata->cluster_bytes = cluster_bytes;
	metadata->fn_cluster_next = fat16_cluster_next;
	metadata->fn_cluster_status = fat16_cluster_status;
	metadata->fn_rootdir_open = fat16_rootdir_open;
	return (KERNEL_SUCCESS);
}

int	fat16_probe(const t_vdl_disk *disk, const t_phy_partition *phy_part, t_fat_metadata *metadata, enum e_fat_errno *fat_err_code)
{
	const uint32_t	partition_addr = SECTORS_TO_BYTES(phy_part->lba_start);
	t_phy_fat16_bpb	fat16_bpb;
	int				err_code;
	int				fat16_err_code;

	err_code = vdl_read(disk, partition_addr, &fat16_bpb, sizeof(fat16_bpb));
	if (err_code != KERNEL_SUCCESS)
	{
		SAFE_PTRSET(fat_err_code, 0);
		return (err_code);
	}
	fat16_err_code = fat16_probe_bpb(&fat16_bpb);
	if (fat16_err_code != KERNEL_SUCCESS)
	{
		SAFE_PTRSET(fat_err_code, fat16_err_code);
		return (-EBADFS);
	}
	err_code = fat16_metadata_init(disk, phy_part, &fat16_bpb, metadata);
	if (err_code != KERNEL_SUCCESS)
	{
		SAFE_PTRSET(fat_err_code, 0);
		return (err_code);
	}
	return (KERNEL_SUCCESS);
}
