/*
 * File: ata.h
 * Author: Joel Souza
 * Date: 2026-03-03
 * Description: ATA controller constants, port offsets and function prototypes
 */

#ifndef ATA_H
# define ATA_H

// ATA base ports
# define ATAB_BUS1		0x1F0
# define ATAB_BUS2		0x170

// ATA ports offset
# define ATAO_DATA		0x0
# define ATAO_NSECTORS	0x2
# define ATAO_LBA_LOW	0x3
# define ATAO_LBA_MID	0x4
# define ATAO_LBA_HIGH	0x5
# define ATAO_DRIVE		0x6
# define ATAO_CMD		0x7

#endif
