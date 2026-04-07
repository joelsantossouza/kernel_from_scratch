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
# include "drivers/disk/disk.h"
# include "autoconfig.h"

# define BYTES_TO_CACHE_LBA(bytes) ({ \
	__typeof__(bytes) _bytes = (bytes); \
	_bytes -= _bytes % CONFIG_VDL_CACHE_ENTRY_BYTES; \
	_bytes / DISK_SECTOR_BYTES; \
})

typedef struct s_vdl_cache
{
	uint8_t		data[CONFIG_VDL_CACHE_ENTRY_BYTES];
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

extern const t_vdl_driver	g_ata_driver;

// Disk operation abstractions
int		vdl_read(const t_vdl_disk *disk, uint32_t addr, void *buf, uint32_t bytes);

// Disk cache
void	vdl_cache_cycle_reset(void);
void	vdl_cache_select(const t_vdl_disk *disk, uint32_t addr, t_vdl_cache **selected);
int		vdl_cache_update(const t_vdl_disk *disk, t_vdl_cache *cache, uint32_t addr, uint32_t lba_end);

#endif
