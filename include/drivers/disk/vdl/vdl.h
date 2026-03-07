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
	uint32_t	addr_start;
	uint32_t	addr_end;
	uint32_t	cycle;
}	t_vdl_cache;

typedef struct s_vdl_disk
{
	uint8_t	no;
}	t_vdl_disk;

#endif
