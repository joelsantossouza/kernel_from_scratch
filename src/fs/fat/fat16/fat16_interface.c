/*
 * File: fat16_interface.c
 * Author: Joel Souza
 * Date: 2026-03-13
 * Description: FAT16 interface structure to implement the VFS protocol
 */

#include "fs/vfs/vfs_interface.h"
#include "fs/fat/fat16/fat16.h"

const t_vfs_interface	g_fat16_interface = {
	.probe = fat16_probe,
};
