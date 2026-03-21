/*
 * File: fat.h
 * Author: Joel Souza
 * Date: 2026-03-12
 * Description: General interface and structures to FAT filesystems familiy
 */

#ifndef FAT_H
# define FAT_H

# include <stdint.h>

// Jump boot opcode
# define FAT_JMP_OPCODE_SHORT		0xEB
# define FAT_JMP_OPCODE_NEAR		0xE9
# define FAT_NOP_OPCODE				0x90

// Media types
# define FAT_MEDIA_FIXED			0xF8
# define FAT_MEDIA_REMOVABLE		0xF0

// FAT errors
# define FAT_EBAD_JMPBOOT			1
# define FAT_EBAD_BYTES_PER_SECT	2
# define FAT_EBAD_SECTS_PER_CLUS	3
# define FAT_EBAD_RSVD_SECTS_CNT	4
# define FAT_EBAD_FATS_CNT			5
# define FAT_EBAD_TOTAL_SECTS		6
# define FAT_EBAD_MEDIA				7

// FAT clusters
# define FAT_CLUSTS_RSVD			2

typedef struct s_phy_fat_file
{
	uint8_t		name[8];
	uint8_t		ext[3];
	uint8_t		attr;
	uint8_t		reserved1;
	uint8_t		creation_time_ms;
	uint16_t	creation_time;
	uint16_t	creation_date;
	uint16_t	accessed_date;
	uint16_t	cluster_high16bits;
	uint16_t	modified_time;
	uint16_t	modified_date;
	uint16_t	cluster_low16bits;
	uint32_t	bytes;
} __attribute__((packed))	t_phy_fat_file;

typedef struct s_fat_metadata
{
	union
	{
		uint16_t	fat16[0xffff];	 // WARNING: Future implementation with malloc
		uint32_t	fat32[1];	 // WARNING: Future implementation with malloc
	}	table;
	uint32_t	root_dir;
	uint32_t	cluster_base;
	uint32_t	cluster_bytes;
}	t_fat_metadata;

#endif
