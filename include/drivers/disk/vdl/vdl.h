/*
 * File: vdl.h
 * Author: Joel Souza
 * Date: 2026-03-03
 * Description: Virtual Disk Layer (VDL) structures and abstractions
 */

#ifndef VDL_H
# define VDL_H

# include <stdint.h>
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

typedef uint16_t (*t_disk_read_f)(uint8_t disk_no, uint32_t lba,
								uint16_t *buf, uint8_t nsectors);
typedef struct s_vdl_driver
{
	t_disk_read_f	read;
}	t_vdl_driver;

typedef struct s_vdl_disk
{
	const t_vdl_driver	*driver;
	uint8_t				no;
}	t_vdl_disk;

#endif
