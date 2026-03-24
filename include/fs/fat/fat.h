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
# define FAT_JMP_OPCODE_SHORT	0xEB
# define FAT_JMP_OPCODE_NEAR	0xE9
# define FAT_NOP_OPCODE			0x90

// Media types
# define FAT_MEDIA_FIXED		0xF8
# define FAT_MEDIA_REMOVABLE	0xF0

// FAT clusters
# define FAT_CLUSTER_DATA_START	2

enum e_fat_errno
{
	// FAT errors
	FAT_EBAD_JMPBOOT = 1,
	FAT_EBAD_BYTES_PER_SECT,
	FAT_EBAD_SECTS_PER_CLUS,
	FAT_EBAD_RSVD_SECTS_CNT,
	FAT_EBAD_FATS_CNT,
	FAT_EBAD_TOTAL_SECTS,
	FAT_EBAD_MEDIA,

	// FAT16 errors
	FAT16_EBAD_ROOT_ENTS_CNT,
	FAT16_EBAD_FAT_SECTS,
};

enum e_fat_cluster_stat
{
	FAT_CLUSTER_FREE = 0,
	FAT_CLUSTER_USED,
	FAT_CLUSTER_RSVD,
	FAT_CLUSTER_INV,
	FAT_CLUSTER_BAD,
	FAT_CLUSTER_EOC,
};

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

typedef struct s_vfs_partition	t_vfs_partition;
typedef enum e_fat_cluster_stat	(*t_fat_cluster_next_fn)(const t_vfs_partition *part, uint32_t cluster, uint32_t *next);

typedef struct s_fat_metadata
{
	union
	{
		uint16_t			fat16[0xff];	 // WARNING: Future implementation with malloc
		uint32_t			fat32[1];	 // WARNING: Future implementation with malloc
	}	table;
	uint32_t				table_entries;
	uint32_t				root_dir;
	uint32_t				data_region;
	uint32_t				cluster_bytes;
	t_fat_cluster_next_fn	fn_cluster_next;
}	t_fat_metadata;

#endif
