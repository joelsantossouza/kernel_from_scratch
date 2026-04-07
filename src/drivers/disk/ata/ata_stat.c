/*
 * File: ata_stat.c
 * Author: Joel Souza
 * Date: 2026-03-10
 * Description: ATA status and error conversion functions
 */

#include "drivers/disk/ata/ata.h"
#include "errno.h"

/*
 * ata_to_errno - Converts ATA errors to errno
 *
 * DESCRIPTION
 * 	Converts ATA error status set on ATA error
 * 	register to the equivalent errno value.
 * 	
 * 	Returns positive errno. Caller is responsible
 * 	for negating.
 *
 * RETURN VALUE
 * 	0 == success
 * 	ETIME == ATA controller request timeout
 * 	ENXIO == CHS address not found on disk
 * 	EIO == I/O error, uncorrectable data, bad block detected
 * 	EREMCHG == Media changed during read
 * 	ENOENT == LBA ID not found on disk
 * */
int	ata_to_errno(int ata_err)
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
