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
	VDL_SECTOR_WORDS	=   VDL_SECTOR_BYTES / 2
};

#endif
