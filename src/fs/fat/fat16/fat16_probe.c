/*
 * File: fat16_probe.c
 * Author: Joel Souza
 * Date: 2026-03-12
 * Description: FAT16 detection and validation
 */

#include "kernel/config.h"
#include "fs/fat/fat.h"
#include "fs/fat/fat16/fat16.h"
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

int	fat16_probe(const t_vdl_disk *disk, uint32_t lba, t_fat16_metadata *metadata, int *fs_err_code)
{
	t_phy_fat16_bpb	fat16_bpb;
	int				err_code;
	int				fat16_err_code;

	err_code = disk_vdl_read(disk, SECTORS_TO_BYTES(lba), &fat16_bpb, sizeof(fat16_bpb));
	if (err_code != KERNEL_SUCCESS)
	{
		SAFE_PTRSET(fs_err_code, 0);
		return (err_code);
	}
	fat16_err_code = fat16_probe_bpb(&fat16_bpb);
	if (fat16_err_code != KERNEL_SUCCESS)
	{
		SAFE_PTRSET(fs_err_code, fat16_err_code);
		return (-EBADFS);
	}
	return (KERNEL_SUCCESS);
}
