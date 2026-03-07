/*
 * File: config.h
 * Author: Joel Souza
 * Date: 2026-03-03
 * Description: Virtual Disk Layer (VDL) configuration constants for hard disk management
 */

#ifndef VDL_CONFIG_H
# define VDL_CONFIG_H

# define VDL_SECTOR_BYTES	512

enum e_vdl_info
{
	VDL_SECTOR_WORDS	= VDL_SECTOR_BYTES / 2
};

# define VDL_CACHE_MAX		8
# define VDL_CACHE_SECTORS	1

enum e_vdl_cache_info
{
	VDL_CACHE_BYTES		= VDL_CACHE_SECTORS * VDL_SECTOR_BYTES,
};

#endif
