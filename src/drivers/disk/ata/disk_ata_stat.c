/*
 * File: disk_ata_stat.c
 * Author: Joel Souza
 * Date: 2026-03-10
 * Description: ATA status and error conversion functions
 */

#include "drivers/disk/ata/ata.h"
#include "errno.h"

int	disk_ata_to_errno(int ata_err)
{
	if (ata_err == ATAE_SUCCESS)
		return (0);
	if (ata_err & ATAE_TIMEOUT)
		return (ETIME);
	if (ata_err & (ATAE_AMNF | ATAE_TKZNF))
		return (ENXIO);
	if (ata_err & (ATAE_ABRT | ATAE_UNC | ATAE_BBK))
		return (EIO);
	if (ata_err & (ATAE_MCR | ATAE_MC))
		return (EREMCHG);
	if (ata_err & ATAE_IDNF)
		return (ENOENT);
	return (EIO);
}
