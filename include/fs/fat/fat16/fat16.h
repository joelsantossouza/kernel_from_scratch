/*
 * File: fat16.h
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: FAT16 header and metadata structures implementation
 */

#ifndef FAT16_H
# define FAT16_H

# include <stdint.h>

# define IS_FAT16_CLUSTER_FREE(cluster) ({ \
	__typeof__(cluster) _cluster = (cluster); \
	_cluster == 0x0000; \
})

# define IS_FAT16_CLUSTER_RSVD(cluster) ({ \
	__typeof__(cluster) _cluster = (cluster); \
	_cluster == 0x0001 || (_cluster >= 0xFFF0 && _cluster <= 0xFFF6); \
})

# define IS_FAT16_CLUSTER_USED(cluster) ({ \
	__typeof__(cluster) _cluster = (cluster); \
	_cluster >= 0x0002 && _cluster <= 0xFFEF; \
})

# define IS_FAT16_CLUSTER_BAD(cluster) ({ \
	__typeof__(cluster) _cluster = (cluster); \
	_cluster == 0xFFF7; \
})

# define IS_FAT16_CLUSTER_EOC(cluster) ({ \
	__typeof__(cluster) _cluster = (cluster); \
	_cluster >= 0xFFF8; \
})

typedef struct s_phy_fat16_bpb
{
	uint8_t		jmpboot[3];
	uint8_t		oem_name[8];
	uint16_t	bytes_per_sector;
	uint8_t		sectors_per_cluster;
	uint16_t	reserved_sectors_count;
	uint8_t		fats_count;
	uint16_t	root_entry_count;
	uint16_t	total_sectors_16;
	uint8_t		media;
	uint16_t	fat_sectors_16;
	uint16_t	sectors_per_track;
	uint16_t	heads_count;
	uint32_t	hidden_sectors;
	uint32_t	total_sectors_32;
} __attribute__((packed))	t_phy_fat16_bpb;

typedef struct s_phy_fat16_ebpb
{
	uint8_t		drive_number;
	uint8_t		reserved1;
	uint8_t		boot_signature;
	uint32_t	volume_id;
	uint8_t		volume_label[11];
	uint8_t		fs_type[8];
} __attribute__((packed))	t_phy_fat16_ebpb;

#endif
