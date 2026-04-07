/*
 * File: ata_io.c
 * Author: Joel Souza
 * Date: 2026-04-07
 * Description: Low-level I/O ports ATA controller communication
 */

#include <stdint.h>
#include "drivers/disk/ata/ata.h"
#include "drivers/io/io.h"
#include "autoconfig.h"

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
uint16_t	ata_poll(uint16_t bus_port, uint8_t wait_mask, uint8_t wait_value, uint16_t *err_code)
{
	const uint16_t	ata_status_register = IOP(bus_port, ATAO_STATUS);
	uint8_t			ata_status;
	uint32_t		timeout;

	timeout = CONFIG_ATA_TIMEOUT_CYCLES;
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
