/*
 * File: config.h
 * Author: Joel Souza
 * Date: 2026-03-03
 * Description: Virtual Disk Layer (VDL) configuration constants for hard disk management
 */

#ifndef VDL_CONFIG_H
# define VDL_CONFIG_H

// SECTORS
# define VDL_SECTOR_BYTES	512

enum e_vdl_info
{
	VDL_SECTOR_WORDS	= VDL_SECTOR_BYTES / 2
};

# define BYTES_TO_SECTORS_FLOOR(bytes) ({ \
	__typeof__(bytes) _bytes = (bytes); \
	_bytes / VDL_SECTOR_BYTES; \
})

# define BYTES_TO_SECTORS_CEIL(bytes) ({ \
	__typeof__(bytes) _bytes = (bytes); \
	(VDL_SECTOR_BYTES - 1 + _bytes) / VDL_SECTOR_BYTES; \
})

# define SECTORS_TO_BYTES(sectors) ({ \
	__typeof__(sectors) _sectors = (sectors); \
	_sectors * VDL_SECTOR_BYTES; \
})

// CACHE
# define VDL_CACHE_MAX		8
# define VDL_CACHE_SECTORS	2

enum e_vdl_cache_info
{
	VDL_CACHE_BYTES		= VDL_CACHE_SECTORS * VDL_SECTOR_BYTES,
};

# define BYTES_TO_CACHE_LBA(bytes) ({ \
	__typeof__(bytes) _bytes = (bytes); \
	_bytes -= _bytes % VDL_CACHE_BYTES; \
	_bytes / VDL_SECTOR_BYTES; \
})

#endif
