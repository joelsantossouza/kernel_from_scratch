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

static t_vdl_cache	g_vdl_cache[VDL_CACHE_MAX] = {0};
static uint32_t		g_vdl_cache_cycle = 0;

/*
 * vdl_cache_cycle_reset - Reset cache entry cycles
 *
 * DESCRIPTION
 * 	Sets all g_vdl_cache[i]->cycle to 0 and resets the global
 * 	cache cycle counter (g_vdl_cache_cycle).
 *
 * RETURN VALUE
 * 	None
 * */
static inline
void	vdl_cache_cycle_reset(void)
{
	uint8_t	i;

	i = -1;
	while (++i < VDL_CACHE_MAX)
		g_vdl_cache[i].cycle = 0;
	g_vdl_cache_cycle = 0;
}

/*
 * vdl_cache_select - Selects a cache entry for 'addr'
 *
 * DESCRIPTION
 * 	Searches in g_vdl_cache[] for an entry matching disk number
 * 	and cache-aligned LBA corresponding to 'addr'. If match found,
 * 	sets '*selected' to matching cache entry.
 *
 * 	If no matching found, '*selected' is set to least recently used
 * 	cache (the one with smallest cycle value). That entry is marked as
 * 	free by setting oldest->is_free.
 *
 * RETURN VALUE
 * 	None.
 * */
static inline
void	vdl_cache_select(const t_vdl_disk *disk, uint32_t addr, t_vdl_cache **selected)
{
	const uint8_t	disk_no = disk->no;
	const uint32_t	find_lba_start = BYTES_TO_CACHE_LBA(addr);
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
		if (g_vdl_cache[i].lba_start != find_lba_start)
			continue ;
		*selected = &g_vdl_cache[i];
		return ;
	}
	oldest->is_free = true;
	*selected = oldest;
}

/*
 * vdl_cache_update - Updates cache data
 *
 * DESCRIPTION
 * 	If cache->is_free, the 'cache' is fully reinitialized with
 * 	new disk data, starting at 'addr' aligned down to VDL_CACHE_BYTES.
 *
 * 	If cache already holds data and 'lba_end' exceeds the current
 * 	cache's data end, new sectors are appended up to VDL_CACHE_BYTES
 * 	capacity.
 *
 * 	If cached data already covers 'lba_end', returns immediately.
 *
 * LRU CACHE
 * 	On every call, cache->cycle is set to g_vdl_cache_cycle current
 * 	value and g_vdl_cache_cycle is incremented. If g_vdl_cache_cycle
 * 	reaches UINT32_MAX, all cache cycles are reset to 0.
 *
 * RETURN VALUE
 * 	Return value is propagated from the disk driver read call.
 * 	0 == success
 * 	-ETIME == ATA controller request timeout
 * 	-ENXIO == CHS address not found on disk
 * 	-EIO == I/O error, uncorrectable data, bad block detected
 * 	-EREMCHG == Media changed during read
 * 	-ENOENT == LBA ID not found on disk
 * */
static inline
int	vdl_cache_update(const t_vdl_disk *disk, t_vdl_cache *cache, uint32_t addr, uint32_t lba_end)
{
	const t_vdl_driver	*driver = disk->driver;
	int					read_status;
	void				*cache_offset;

	cache->cycle = g_vdl_cache_cycle++;
	if (g_vdl_cache_cycle == UINT32_MAX)
		vdl_cache_cycle_reset();
	if (cache->is_free == true)
	{
		cache->is_free = false;
		cache->lba_start = BYTES_TO_CACHE_LBA(addr);
		cache->lba_end = cache->lba_start;
	}
	else if (lba_end <= cache->lba_end)
		return (KERNEL_SUCCESS);
	lba_end = MIN(lba_end, cache->lba_start + VDL_CACHE_SECTORS);
	cache_offset = cache->data + SECTORS_TO_BYTES(cache->lba_end - cache->lba_start);
	read_status = driver->read(
		disk->no, cache->lba_end, cache_offset, lba_end - cache->lba_end
	);
	cache->lba_end = lba_end;
	return (-driver->to_errno(read_status));
}

/*
 * disk_vdl_read - Disk read abstraction
 *
 * DESCRIPTION
 * 	Read 'bytes' bytes from disk offset 'addr' into 'buf'.
 * 	Uses disk drivers polymorphism for compatibility with any disk type.
 *
 * IMPLEMENTATION
 * 	Cache optimization to decrease number of requests to drivers.
 * 	g_vdl_cache is the table of VDL_CACHE_MAX entries, each storing
 * 	VDL_CACHE_BYTES bytes. If cache is full, it selects the oldest
 * 	entry to overwrite it.
 *
 * 	Three read phases:
 * 	1. Partial read to align addr to VDL_CACHE_BYTES.
 * 	2. Full VDL_CACHE_BYTES chunks read.
 * 	3. Remainder bytes of a cache-chunk.
 *
 * RETURN VALUE
 * 	0 == success
 * 	-ETIME == ATA controller request timeout
 * 	-ENXIO == CHS address not found on disk
 * 	-EIO == I/O error, uncorrectable data, bad block detected
 * 	-EREMCHG == Media changed during read
 * 	-ENOENT == LBA ID not found on disk
 * */
int	disk_vdl_read(const t_vdl_disk *disk, uint32_t addr, void *buf, uint32_t bytes)
{
	const uint32_t	lba_end = BYTES_TO_SECTORS_CEIL(addr + bytes);
	const uint32_t	addr_unalign = OFFSET(addr, VDL_CACHE_BYTES);
	t_vdl_cache		*cache;
	uint32_t		bytes_to_read;
	int				err_code;

	if (addr_unalign)
	{
		vdl_cache_select(disk, addr, &cache);
		err_code = vdl_cache_update(disk, cache, addr, lba_end);
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
		err_code = vdl_cache_update(disk, cache, addr, lba_end);
		if (err_code != KERNEL_SUCCESS)
			return (err_code);
		buf = mempcpy(buf, cache->data, VDL_CACHE_BYTES);
		addr += VDL_CACHE_BYTES;
		bytes -= VDL_CACHE_BYTES;
	}
	if (bytes > 0)
	{
		vdl_cache_select(disk, addr, &cache);
		err_code = vdl_cache_update(disk, cache, addr, lba_end);
		if (err_code != KERNEL_SUCCESS)
			return (err_code);
		mempcpy(buf, cache->data, bytes);
	}
	return (KERNEL_SUCCESS);
}
