/*
 * File: disk_ata_io.c
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: Drivers to Read and Write in hard disk using ATA Controller
 */

#include <stdint.h>
#include <sys/types.h>
#include "drivers/disk/vdl/vdl.h"
#include "drivers/io/io.h"
#include "drivers/disk/ata/ata.h"

static inline
void	disk_ata_exec(uint8_t disk_no, uint8_t cmd, uint32_t lba, uint8_t nsectors)
{
	const uint16_t	bus_port = disk_no < 2 ? ATAB_BUS1 : ATAB_BUS2;
	const uint8_t	flags = ATAF_LBA_MODE | ((disk_no & 0x01) << 4);

	io_outb(IOP(bus_port, ATAO_DRIVE),		(uint8_t)((lba >> 24) | flags));
	io_outb(IOP(bus_port, ATAO_NSECTORS),	(uint8_t)nsectors);
	io_outb(IOP(bus_port, ATAO_LBA_LOW),	(uint8_t)(lba & 0xff));
	io_outb(IOP(bus_port, ATAO_LBA_MID),	(uint8_t)(lba >> 8));
	io_outb(IOP(bus_port, ATAO_LBA_HIGH),	(uint8_t)(lba >> 16));
	io_outb(IOP(bus_port, ATAO_CMD),		(uint8_t)cmd);
}

ssize_t	disk_ata_read(const t_vdl_disk *disk, uint32_t lba, uint16_t *buf, uint8_t nsectors)
{
}
