/*
 * File: fat.h
 * Author: Joel Souza
 * Date: 2026-03-12
 * Description: General interface and structures to FAT filesystems familiy
 */

#ifndef FAT_H
# define FAT_H

// Jump boot opcode
# define FAT_JMP_OPCODE_SHORT	0xEB
# define FAT_JMP_OPCODE_NEAR	0xE9
# define FAT_NOP_OPCODE			0x90

// Media types
# define FAT_MEDIA_FIXED		0xF8
# define FAT_MEDIA_REMOVABLE	0xF0

// FAT errors
# define FAT_EBAD_JMPBOOT			1
# define FAT_EBAD_BYTES_PER_SECT	2
# define FAT_EBAD_SECTS_PER_CLUS	3
# define FAT_EBAD_RSVD_SECTS_CNT	4
# define FAT_EBAD_FATS_CNT			5
# define FAT_EBAD_TOTAL_SECTS		6
# define FAT_EBAD_MEDIA				7

#endif
