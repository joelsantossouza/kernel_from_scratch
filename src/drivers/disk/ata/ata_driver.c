/*
 * File: ata_driver.c
 * Author: Joel Souza
 * Date: 2026-04-07
 * Description: ATA driver interface that follows the VDL driver protocol
 */

#include "drivers/disk/vdl/vdl.h"
#include "drivers/disk/ata/ata.h"

const t_vdl_driver	g_ata_driver = {
	.read = ata_read,
	.to_errno = ata_to_errno,
};
