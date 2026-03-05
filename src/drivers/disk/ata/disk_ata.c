/*
 * File: disk_ata.c
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: Drivers to manage hard disk using ATA Controller
 */

#include <stdint.h>
#include "drivers/io/io.h"
#include "drivers/disk/vdl/config.h"
#include "drivers/disk/ata/ata.h"
#include "drivers/disk/ata/config.h"

/*
 * ata_request_rw - Request Read/Write on disk data via ATA controller
 *
 * DESCRIPTION
 *	Sends a read or write request to ATA controller setting the
 *	LBA address, sector count and command into ATA registers.
 *
 *	bus_port	-> ATAB_BUS1 or ATAB_BUS2
 *	drive		-> 0 = Master, 1 = Slave
 *	rw_cmd		-> ATAC_READ or ATAC_WRITE
 *	lba			-> starting sector index
 *	nsectors	-> number of sectors to read/write
 *
 * RETURN VALUE
 *	None
 * */
static inline
void	ata_request_rw(uint16_t bus_port, uint8_t drive, uint8_t rw_cmd, uint32_t lba, uint8_t nsectors)
{
	const uint8_t	flags = ATAF_LBA_MODE | ((drive & 0x01) << 4);

	io_outb(IOP(bus_port, ATAO_DRIVE),		(uint8_t)((lba >> 24) | flags));
	io_outb(IOP(bus_port, ATAO_NSECTORS),	(uint8_t)nsectors);
	io_outb(IOP(bus_port, ATAO_LBA_LOW),	(uint8_t)(lba & 0xff));
	io_outb(IOP(bus_port, ATAO_LBA_MID),	(uint8_t)(lba >> 8));
	io_outb(IOP(bus_port, ATAO_LBA_HIGH),	(uint8_t)(lba >> 16));
	io_outb(IOP(bus_port, ATAO_CMD),		(uint8_t)rw_cmd);
}

/*
 * ata_poll - Helper function to wait ATA controller status flags
 *
 * DESCRIPTION
 * 	Polls the status register of a given ATA bus port, waiting
 * 	until the ata_status masked with wait_mask equals wait_value.
 *
 * 	ata_poll waits for a limited number of cycles (ATA_TIMEOUT_CYCLES).
 *
 * RETURN VALUE
 * 	On success returns 0 (ATAE_SUCCESS). On failure, sets err_code
 * 	with the respective ATAE_* error code and returns ATAE_FAIL.
 *
 * 	If the waiting time exceeds the ATA_TIMEOUT_CYCLES, err_code is
 * 	set to ATAE_TIMEOUT.
 * */
static inline
uint16_t	ata_poll(uint16_t bus_port, uint8_t wait_mask,
					uint8_t wait_value, uint16_t *err_code)
{
	const uint16_t	ata_status_register = IOP(bus_port, ATAO_STATUS);
	uint8_t			ata_status;
	uint32_t		timeout;

	timeout = ATA_TIMEOUT_CYCLES;
	while (timeout-- > 0)
	{
		ata_status = io_inb(ata_status_register);
		if (ata_status & ATAS_HAS_ERROR)
		{
			*err_code = io_inb(IOP(bus_port, ATAO_ERROR));
			return (ATAE_FAIL);
		}
		if ((ata_status & wait_mask) == wait_value)
			return (ATAE_SUCCESS);
	}
	*err_code = ATAE_TIMEOUT;
	return (ATAE_FAIL);
}

/*
 * disk_ata_read - ATA controller disk read
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
uint16_t	disk_ata_read(uint8_t disk_no, uint32_t lba,
						uint16_t *buf, uint8_t nsectors)
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
		words_remaining = VDL_SECTOR_WORDS;
		while (words_remaining-- > 0)
			*buf++ = io_inw(ata_data_register);
	}
	return (ATAE_SUCCESS);
}
