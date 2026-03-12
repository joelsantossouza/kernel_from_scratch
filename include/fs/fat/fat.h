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

#endif
