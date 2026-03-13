/*
 * File: vdl.h
 * Author: Joel Souza
 * Date: 2026-03-03
 * Description: Virtual Disk Layer (VDL) structures and abstractions
 */

#ifndef VDL_H
# define VDL_H

# include <stdint.h>
# include <stdbool.h>
# include "drivers/disk/vdl/config.h"

typedef struct s_vdl_cache
{
	uint8_t		data[VDL_CACHE_BYTES];
	uint32_t	lba_start;
	uint32_t	lba_end;
	uint32_t	cycle;
	uint8_t		disk_no;
	bool		is_free;
}	t_vdl_cache;

typedef uint16_t (*t_vdl_read_fn)(uint8_t disk_no, uint32_t lba,
								uint16_t *buf, uint8_t nsectors);
typedef int (*t_vdl_to_errno_fn)(int ata_err);
typedef struct s_vdl_driver
{
	t_vdl_read_fn		read;
	t_vdl_to_errno_fn	to_errno;
}	t_vdl_driver;

typedef struct s_vdl_disk
{
	const t_vdl_driver	*driver;
	uint8_t				no;
}	t_vdl_disk;

int	disk_vdl_read(const t_vdl_disk *disk, uint32_t addr, void *buf, uint32_t bytes);

#endif
