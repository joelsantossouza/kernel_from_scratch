/*
 * File: disk_vdl.c
 * Author: Joel Souza
 * Date: 2026-03-07
 * Description: Virtual Disk Layer interface to communicate with different
 *				disk types using cache to optimize performance.
 */

#include <stdint.h>
#include "drivers/disk/vdl/vdl.h"
#include "drivers/disk/vdl/config.h"

t_vdl_cache	g_vdl_cache[VDL_CACHE_MAX] = {0};

static inline
int	vdl_cache_select(uint32_t addr, t_vdl_cache **selected)
{
	t_vdl_cache	*oldest;
	uint8_t		i;

	oldest = &g_vdl_cache[0];
	i = -1;
	while (++i < VDL_CACHE_MAX)
	{
		if (addr >= g_vdl_cache[i].addr_start && addr < g_vdl_cache[i].addr_end)
		{
			*selected = &g_vdl_cache[i];
			return (0);
		}
		if (g_vdl_cache[i].cycle < oldest->cycle)
			oldest = &g_vdl_cache[i];
	}
	*selected = oldest;
	return (1);
}
