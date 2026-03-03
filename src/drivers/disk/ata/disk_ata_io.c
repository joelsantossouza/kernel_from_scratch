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

ssize_t	disk_ata_read(const t_vdl_disk *disk, uint32_t lba, uint16_t *buf, uint8_t nsectors)
{
}
