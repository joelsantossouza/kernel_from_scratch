/*
 * File: fat.h
 * Author: Joel Souza
 * Date: 2026-03-12
 * Description: General interface and structures to FAT filesystems familiy
 */

#ifndef FAT_H
# define FAT_H

# include <stdint.h>
# include <stdbool.h>

// Jump boot opcode
# define FAT_JMP_OPCODE_SHORT	0xEB
# define FAT_JMP_OPCODE_NEAR	0xE9
# define FAT_NOP_OPCODE			0x90

// Media types
# define FAT_MEDIA_FIXED		0xF8
# define FAT_MEDIA_REMOVABLE	0xF0

// FAT clusters
# define FAT_CLUSTER_DATA_START	2

// FAT directory name symbols
# define FAT_DIR_ENTRY_UNUSED	0x0
# define FAT_DIR_ENTRY_DELETED	0xE5

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

// FAT file
enum e_fat_filename_limits
{
	FAT_FILE_NAME_MAX = 8,
	FAT_FILE_EXT_MAX = 3,
	FAT_FILE_FULLNAME_MAX = FAT_FILE_NAME_MAX + FAT_FILE_EXT_MAX,
};

typedef struct s_phy_fat_file
{
	uint8_t		name[FAT_FILE_NAME_MAX];
	uint8_t		ext[FAT_FILE_EXT_MAX];
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

typedef struct s_fat_file
{
	t_phy_fat_file			entry;
	const t_vfs_partition	*partition;
	uint32_t				start;
	uint32_t				bytes;
}	t_fat_file;

typedef t_fat_file	t_fat_dir;

// FAT metadata
typedef enum e_fat_cluster_stat	(*t_fat_cluster_next_fn)(const t_vfs_partition *part, uint32_t cluster, uint32_t *next);
typedef enum e_fat_cluster_stat	(*t_fat_cluster_status_fn)(uint32_t cluster);
typedef int		(*t_fat_rootdir_open_fn)(const t_fat_dir *dir, const char *filename, const char **filename_next, t_fat_file *file);

typedef struct s_fat_metadata
{
	union
	{
		uint16_t			fat16[0xff];	 // WARNING: Future implementation with malloc
		uint32_t			fat32[1];	 // WARNING: Future implementation with malloc
	}	table;
	uint32_t				table_entries;
	uint32_t				rootdir;
	uint32_t				data_region;
	uint32_t				cluster_bytes;
	t_fat_cluster_next_fn	fn_cluster_next;
	t_fat_cluster_status_fn	fn_cluster_status;
	t_fat_rootdir_open_fn	fn_rootdir_open;
}	t_fat_metadata;

// cluster operations
int		fat_cluster_read(const t_vfs_partition *part, uint32_t *cluster, uint32_t *offset, void *buf, uint32_t bytes);

// dirs operations
int		fat_rootdir_init(const t_vfs_partition *part, t_fat_dir *rootdir);
int		fat_dir_open(const t_fat_dir *dir, const char *filename, const char **filename_next, t_fat_file *file);

// files operations
int		fat_file_init(const t_vfs_partition *part, const t_phy_fat_file *entry, t_fat_file *file);
bool	fat_file_name_match(const t_phy_fat_file *entry, const char *filename, const char **filename_next);

#endif
