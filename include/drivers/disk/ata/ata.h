/*
 * File: ata.h
 * Author: Joel Souza
 * Date: 2026-03-03
 * Description: ATA controller constants, port offsets and function prototypes
 */

#ifndef ATA_H
# define ATA_H

# include <stdint.h>

// ATA Base ports
# define ATAB_BUS1		0x1F0
# define ATAB_BUS2		0x170

// ATA Ports offset
# define ATAO_DATA		0x0
# define ATAO_ERROR		0x1
# define ATAO_NSECTORS	0x2
# define ATAO_LBA_LOW	0x3
# define ATAO_LBA_MID	0x4
# define ATAO_LBA_HIGH	0x5
# define ATAO_DRIVE		0x6
# define ATAO_CMD		0x7
# define ATAO_STATUS	0x7

// ATA Commands
# define ATAC_READ		0x20
# define ATAC_WRITE		0x30

// ATA Flags
# define ATAF_LBA_MODE	0b11100000
# define ATAF_CHS_MODE	0b10100000

// ATA Status bitmasks
# define ATAS_ERROR		0b00000001
# define ATAS_DRQ		0b00001000
# define ATAS_DF		0b00100000
# define ATAS_BSY		0b10000000
# define ATAS_HAS_ERROR	0b00100001

// ATA Errors
# define ATAE_SUCCESS	0x0000
# define ATAE_AMNF		0x0001
# define ATAE_TKZNF		0x0002
# define ATAE_ABRT		0x0004
# define ATAE_MCR		0x0008
# define ATAE_IDNF		0x0010
# define ATAE_MC		0x0020
# define ATAE_UNC		0x0040
# define ATAE_BBK		0x0080
# define ATAE_TIMEOUT	0x0100
# define ATAE_FAIL		0xFFFF

// I/O ports ATA Controller routines
void		ata_request_rw(uint16_t bus_port, uint8_t drive, uint8_t rw_cmd, uint32_t lba, uint8_t nsectors);
uint16_t	ata_poll(uint16_t bus_port, uint8_t wait_mask, uint8_t wait_value, uint16_t *err_code);

// Operations
uint16_t	ata_read(uint8_t disk_no, uint32_t lba, uint16_t *buf, uint8_t nsectors);
int			ata_to_errno(int ata_err);

#endif
