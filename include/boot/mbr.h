/*
 * File: mbr.h
 * Author: Joel Souza
 * Date: 2026-03-11
 * Description: MBR definitions and offsets layout
 */

#ifndef MBR_H
# define MBR_H

// Partition Table
# define MBR_PART_TABLE_ADDR		0x1BE
# define MBR_PART_TABLE_ENTRY_BYTES	16
# define MBR_PART_TABLE(index) ({ \
	__typeof__(index) _index = (index); \
	MBR_PART_TABLE_ADDR + _index * MBR_PART_TABLE_ENTRY_BYTES; \
})

# define MBR_PART_TYPE_FAT16_LBA	0x0E

#endif
