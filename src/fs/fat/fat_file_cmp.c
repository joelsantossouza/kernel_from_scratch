/*
 * File: fat_file_cmp.c
 * Author: Joel Souza
 * Date: 2026-03-25
 * Description: FAT functions for comparing and matching file members
 */

#include <stdbool.h>
#include <stdint.h>
#include "fs/fat/fat.h"
#include "ctype/ctype.h"

bool	fat_file_match_name(const t_phy_fat_file *file, const char *filename, const char **filename_next)
{
	bool	is_fat_done;
	bool	is_path_done;
	uint8_t	fat_chr;
	uint8_t	path_chr;
	uint8_t	i;

	*filename_next = filename;
	for (i = 0; i < FAT_FILE_NAME_MAX; i++)
	{
		fat_chr = file->name[i];
		is_fat_done = fat_chr == ' ';
		path_chr = **filename_next;
		*filename_next += path_chr != 0;
		is_path_done = path_chr == '/' || path_chr == '.' || path_chr == 0;
		if (is_fat_done && is_path_done)
			break ;
		if (is_fat_done != is_path_done || fat_chr != toupper(path_chr))
			return (false);
	}
	for (i = 0; i < FAT_FILE_EXT_MAX; i++)
	{
		fat_chr = file->ext[i];
		is_fat_done = fat_chr == ' ';
		path_chr = **filename_next;
		*filename_next += path_chr != 0;
		is_path_done = path_chr == '/' || path_chr == 0;
		if (is_fat_done && is_path_done)
			return (true);
		if (is_fat_done != is_path_done || fat_chr != toupper(path_chr))
			return (false);
	}
	return (true);
}
