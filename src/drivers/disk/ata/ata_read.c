/*
 * File: ata_read.c
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: Drivers to read from disk via ATA Controller
 */

#include <stdint.h>
#include "drivers/disk/disk.h"
#include "drivers/disk/ata/ata.h"
#include "drivers/io/io.h"

/*
 * ata_read - ATA controller disk read
 *
 * DESCRIPTION
 * 	Read sectors from disk using ATA controller interface. You must provide
 * 	the disk that will be read, the Logical Block Address (LBA) start sector,
 * 	the buffer where the memory will be stored in, and the number of sectors
 * 	to be read.
 *
 * 	buf must have at least nsectors * VDL_SECTOR_BYTES BYTES.
 * 	See disk/vdl/config.h
 *
 * RETURN VALUE
 * 	Fills the buffer with disk data and returns 0 (ATAE_SUCCESS) on success,
 * 	or an ATAE_* error code on failure. See ata.h to check ATA Error values.
 * */
uint16_t	ata_read(uint8_t disk_no, uint32_t lba, uint16_t *buf, uint8_t nsectors)
{
	const uint16_t	bus_port = disk_no < 2 ? ATAB_BUS1 : ATAB_BUS2;
	const uint16_t	ata_data_register = IOP(bus_port, ATAO_DATA);
	const uint8_t	ata_ready_mask = ATAS_BSY | ATAS_DRQ;
	uint16_t		words_remaining;
	uint16_t		err_code;

	if (ata_poll(bus_port, ATAS_BSY, 0, &err_code) == ATAE_FAIL)
		return (err_code);
	ata_request_rw(bus_port, disk_no, ATAC_READ, lba, nsectors);
	while (nsectors-- > 0)
	{
		if (ata_poll(bus_port, ata_ready_mask, ATAS_DRQ, &err_code) == ATAE_FAIL)
			return (err_code);
		words_remaining = DISK_SECTOR_WORDS;
		while (words_remaining-- > 0)
			*buf++ = io_inw(ata_data_register);
	}
	return (ATAE_SUCCESS);
}
