/*
 * File: disk_vdl.c
 * Author: Joel Souza
 * Date: 2026-03-07
 * Description: Virtual Disk Layer interface to communicate with different
 * 				disk types using cache to optimize performance.
 */

#include <stdint.h>
#include <stdbool.h>
#include "drivers/disk/vdl/vdl.h"
#include "drivers/disk/vdl/config.h"
#include "kernel/config.h"
#include "string/string.h"
#include "math/math.h"

t_vdl_cache	g_vdl_cache[VDL_CACHE_MAX] = {0};
uint32_t	g_vdl_cache_cycle = 0;

static inline
void	vdl_cache_cycle_reset(void)
{
	uint8_t	i;

	i = -1;
	while (++i < VDL_CACHE_MAX)
		g_vdl_cache[i].cycle = 0;
	g_vdl_cache_cycle = 0;
}

static inline
void	vdl_cache_select(const t_vdl_disk *disk, uint32_t addr, t_vdl_cache **selected)
{
	const uint8_t	disk_no = disk->no;
	t_vdl_cache		*oldest;
	uint8_t			i;

	oldest = &g_vdl_cache[0];
	i = -1;
	while (++i < VDL_CACHE_MAX)
	{
		if (g_vdl_cache[i].cycle < oldest->cycle)
			oldest = &g_vdl_cache[i];
		if (g_vdl_cache[i].disk_no != disk_no)
			continue ;
		if (addr < g_vdl_cache[i].addr_start || addr >= g_vdl_cache[i].addr_end)
			continue ;
		*selected = &g_vdl_cache[i];
		return ;
	}
	oldest->is_free = true;
	*selected = oldest;
}

static inline
int	vdl_cache_update(const t_vdl_disk *disk, t_vdl_cache *cache, uint32_t addr_start, uint32_t addr_end)
{
	int			read_status;
	uint32_t	lba;
	void		*cache_offset;
	uint32_t	nsectors;

	if (cache->is_free == true)
	{
		cache->is_free = false;
		cache->addr_start = ALIGN_DOWN(addr_start, VDL_CACHE_BYTES);
		cache->addr_end = cache->addr_start;
	}
	else if (addr_end <= cache->addr_end)
		return (KERNEL_SUCCESS);
	addr_end = ALIGN_UP(addr_end, VDL_SECTOR_BYTES);
	addr_end = MIN(addr_end, cache->addr_start + VDL_CACHE_BYTES);

	lba = BYTES_TO_SECTOR(cache->addr_end);
	cache_offset = cache->data + OFFSET(cache->addr_end, VDL_CACHE_BYTES);
	nsectors = BYTES_TO_SECTOR(addr_end - cache->addr_end);
	read_status = disk->driver->read(disk->no, lba, cache_offset, nsectors);
	cache->addr_end = addr_end;
	cache->cycle = g_vdl_cache_cycle++;
	if (g_vdl_cache_cycle == UINT32_MAX)
		vdl_cache_cycle_reset();
	return (-read_status);
}

int	disk_vdl_read(const t_vdl_disk *disk, uint32_t addr, void *buf, uint32_t bytes)
{
	const uint32_t	addr_end = addr + bytes;
	const uint32_t	addr_unalign = OFFSET(addr, VDL_CACHE_BYTES);
	t_vdl_cache		*cache;
	uint32_t		bytes_to_read;
	int				err_code;

	if (addr_unalign)
	{
		vdl_cache_select(disk, addr, &cache);
		err_code = vdl_cache_update(disk, cache, addr, addr_end);
		if (err_code != KERNEL_SUCCESS)
			return (err_code);
		bytes_to_read = MIN(VDL_CACHE_BYTES - addr_unalign, bytes);
		buf = mempcpy(buf, cache->data + addr_unalign, bytes_to_read);
		addr += bytes_to_read;
		bytes -= bytes_to_read;
	}
	while (bytes >= VDL_CACHE_BYTES)
	{
		vdl_cache_select(disk, addr, &cache);
		err_code = vdl_cache_update(disk, cache, addr, addr_end);
		if (err_code != KERNEL_SUCCESS)
			return (err_code);
		buf = mempcpy(buf, cache->data, VDL_CACHE_BYTES);
		addr += VDL_CACHE_BYTES;
		bytes -= VDL_CACHE_BYTES;
	}
	if (bytes > 0)
	{
		vdl_cache_select(disk, addr, &cache);
		err_code = vdl_cache_update(disk, cache, addr, addr_end);
		if (err_code != KERNEL_SUCCESS)
			return (err_code);
		mempcpy(buf, cache->data, bytes);
	}
	return (KERNEL_SUCCESS);
}
