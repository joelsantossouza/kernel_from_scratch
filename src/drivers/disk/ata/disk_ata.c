/*
 * File: disk_ata.c
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: Drivers to manage hard disk using ATA Controller
 */

#include <stdint.h>
#include "drivers/io/io.h"
#include "drivers/disk/vdl/vdl.h"
#include "drivers/disk/vdl/config.h"
#include "drivers/disk/ata/ata.h"
#include "drivers/disk/ata/config.h"

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

static inline
uint16_t	ata_poll(uint16_t bus_port, uint16_t *err_code)
{
	const uint16_t	ata_status_register = IOP(bus_port, ATAO_STATUS);
	const uint8_t	has_error = ATAS_ERROR | ATAS_DF;
	uint8_t			ata_status;
	uint32_t		timeout;

	timeout = ATA_TIMEOUT_CYCLES;
	while (timeout-- > 0)
	{
		ata_status = io_inb(ata_status_register);
		if (ata_status & has_error)
		{
			*err_code = io_inb(IOP(bus_port, ATAO_ERROR));
			return (ATAE_FAIL);
		}
		if ((ata_status & ATAS_BSY) == 0 && (ata_status & ATAS_DRQ))
			return (ATAE_SUCCESS);
	}
	*err_code = ATAE_TIMEOUT;
	return (ATAE_FAIL);
}

uint16_t	disk_ata_read(const t_vdl_disk *disk, uint32_t lba, uint16_t *buf, uint8_t nsectors)
{
	const uint16_t	bus_port = disk->no < 2 ? ATAB_BUS1 : ATAB_BUS2;
	const uint16_t	ata_data_register = IOP(bus_port, ATAO_DATA);
	uint16_t		nwords;
	uint16_t		err_code;

	ata_request_rw(bus_port, disk->no, ATAC_READ, lba, nsectors);
	while (nsectors-- > 0)
	{
		if (ata_poll(bus_port, &err_code) == ATAE_FAIL)
			return (err_code);
		nwords = VDL_SECTOR_WORDS;
		while (nwords-- > 0)
			*buf++ = io_inw(ata_data_register);
	}
	return (ATAE_SUCCESS);
}
