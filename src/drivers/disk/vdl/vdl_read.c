/*
 * File: vdl_read.c
 * Author: Joel Souza
 * Date: 2026-03-07
 * Description: VDL interface to abstract reading from disk
 */

#include <stdint.h>
#include "drivers/disk/vdl/vdl.h"
#include "string/string.h"
#include "math/math.h"
#include "kernel/config.h"

/*
 * vdl_read - Disk read abstraction
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
int	vdl_read(const t_vdl_disk *disk, uint32_t addr, void *buf, uint32_t bytes)
{
	const uint32_t	lba_end = BYTES_TO_SECTORS_CEIL(addr + bytes);
	const uint32_t	addr_unalign = addr % CONFIG_VDL_CACHE_ENTRY_BYTES;
	t_vdl_cache		*cache;
	uint32_t		bytes_to_read;
	int				err_code;

	if (addr_unalign)
	{
		vdl_cache_select(disk, addr, &cache);
		err_code = vdl_cache_update(disk, cache, addr, lba_end);
		if (err_code != KERNEL_SUCCESS)
			return (err_code);
		bytes_to_read = MIN(CONFIG_VDL_CACHE_ENTRY_BYTES - addr_unalign, bytes);
		buf = mempcpy(buf, cache->data + addr_unalign, bytes_to_read);
		addr += bytes_to_read;
		bytes -= bytes_to_read;
	}
	while (bytes >= CONFIG_VDL_CACHE_ENTRY_BYTES)
	{
		vdl_cache_select(disk, addr, &cache);
		err_code = vdl_cache_update(disk, cache, addr, lba_end);
		if (err_code != KERNEL_SUCCESS)
			return (err_code);
		buf = mempcpy(buf, cache->data, CONFIG_VDL_CACHE_ENTRY_BYTES);
		addr += CONFIG_VDL_CACHE_ENTRY_BYTES;
		bytes -= CONFIG_VDL_CACHE_ENTRY_BYTES;
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
