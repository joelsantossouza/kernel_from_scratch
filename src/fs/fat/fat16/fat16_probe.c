/*
 * File: fat16_probe.c
 * Author: Joel Souza
 * Date: 2026-03-12
 * Description: FAT16 detection and validation
 */

#include "fs/fat/fat.h"
#include "fs/fat/fat16/fat16.h"
#include "math/math.h"

static inline
int	fat16_probe_bpb(const t_phy_fat16_bpb *bpb)
{
	if ((bpb->jmpboot[0] != FAT_JMP_OPCODE_SHORT || bpb->jmpboot[2] != FAT_NOP_OPCODE)
		&& bpb->jmpboot[0] != FAT_JMP_OPCODE_NEAR)
		return (...);
	if (bpb->bytes_per_sector != 512 && bpb->bytes_per_sector != 1024
		&& bpb->bytes_per_sector != 2048 && bpb->bytes_per_sector != 4096)
		return (...);
	if (bpb->sectors_per_cluster == 0 || !IS_POWER_2(bpb->sectors_per_cluster))
		return (...);
	if (bpb->reserved_sectors_count == 0)
		return (...);
	if (bpb->fats_count == 0)
		return (...);
	if (bpb->root_entry_count == 0)
		return (...);
	if (bpb->total_sectors_16 == 0 && bpb->total_sectors_32 == 0)
		return (...);
	if (bpb->media != FAT_MEDIA_FIXED && bpb->media != FAT_MEDIA_REMOVABLE)
		return (...);
	if (bpb->fat_sectors_16 == 0)
		return (...);
}

int	fat16_probe(const t_vdl_disk *disk, uint32_t lba, t_vfs_metadata *metadata)
{
	t_phy_fat16_bpb	fat16_bpb;
	int				err_code;

	err_code = disk_vdl_read(disk, SECTORS_TO_BYTES(lba), &fat16_bpb, sizeof(fat16_bpb));
	if (err_code != KERNEL_SUCCESS)
		return (err_code);

}
