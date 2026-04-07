/*
 * File: disk.h
 * Author: Joel Souza
 * Date: 2026-04-07
 * Description: Disk geometry constants and arithmetic macros
 */

#ifndef DISK_H
# define DISK_H

enum e_disk_geometry
{
	DISK_SECTOR_BYTES = 512,
	DISK_SECTOR_WORDS = DISK_SECTOR_BYTES / 2,
};

# define BYTES_TO_SECTORS_FLOOR(bytes) ({ \
	__typeof__(bytes) _bytes = (bytes); \
	_bytes / DISK_SECTOR_BYTES; \
})

# define BYTES_TO_SECTORS_CEIL(bytes) ({ \
	__typeof__(bytes) _bytes = (bytes); \
	(DISK_SECTOR_BYTES - 1 + _bytes) / DISK_SECTOR_BYTES; \
})

# define SECTORS_TO_BYTES(sectors) ({ \
	__typeof__(sectors) _sectors = (sectors); \
	_sectors * DISK_SECTOR_BYTES; \
})

#endif
