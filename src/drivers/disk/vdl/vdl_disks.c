/*
 * File: vdl_disks.c
 * Author: Joel Souza
 * Date: 2026-04-07
 * Description: Virtual Disk Layer (VDL) structures abstracting physical disk attributes
 */

#include "drivers/disk/vdl/vdl.h"

const t_vdl_disk	g_disk0 = {
	.driver	= &g_ata_driver,
	.no		= 0,
};
