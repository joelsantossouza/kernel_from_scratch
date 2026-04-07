/*
 * File: vdl_cache.c
 * Author: Joel Souza
 * Date: 2026-04-07
 * Description: Disk cache implementation to optimize read/write operations
 */

#include "drivers/disk/vdl/vdl.h"
#include "math/math.h"
#include "kernel/kernel.h"

static t_vdl_cache	g_vdl_cache[CONFIG_VDL_CACHE_ENTRY_COUNT] = {0};
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
void	vdl_cache_cycle_reset(void)
{
	uint8_t	i;

	i = -1;
	while (++i < CONFIG_VDL_CACHE_ENTRY_COUNT)
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
void	vdl_cache_select(const t_vdl_disk *disk, uint32_t addr, t_vdl_cache **selected)
{
	const uint8_t	disk_no = disk->no;
	const uint32_t	find_lba_start = BYTES_TO_CACHE_LBA(addr);
	t_vdl_cache		*oldest;
	uint8_t			i;

	oldest = &g_vdl_cache[0];
	i = -1;
	while (++i < CONFIG_VDL_CACHE_ENTRY_COUNT)
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
	lba_end = MIN(lba_end, cache->lba_start + CONFIG_VDL_CACHE_ENTRY_SECTORS);
	cache_offset = cache->data + SECTORS_TO_BYTES(cache->lba_end - cache->lba_start);
	read_status = driver->read(
		disk->no, cache->lba_end, cache_offset, lba_end - cache->lba_end
	);
	cache->lba_end = lba_end;
	return (-driver->to_errno(read_status));
}
