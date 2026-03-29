/*
 * File: fat_file_name.c
 * Author: Joel Souza
 * Date: 2026-03-25
 * Description: FAT file name comparison and conversion functions
 */

#include <stdbool.h>
#include <stdint.h>
#include "fs/fat/fat.h"
#include "string/string.h"
#include "ctype/ctype.h"
#include "kernel/config.h"
#include "errno.h"

int	fat_file_name_convert_strict(uint8_t fat_fullname[FAT_FILE_FULLNAME_MAX], const char *filename)
{
	const char	*ptr = filename;
	uint8_t		chr;
	uint8_t		i;

	for (i = 0; i < FAT_FILE_NAME_MAX; i++)
	{
		chr = *ptr;
		if (chr == '.' || chr == '/' || chr == 0)
			break ;
		fat_fullname[i] = toupper(chr);
		ptr++;
	}
	chr = *ptr;
	if (chr != '.' && chr != '/' && chr != 0)
		return (-ENAMETOOLONG);
	for (; i < FAT_FILE_NAME_MAX; i++)
		fat_fullname[i] = ' ';
	ptr += *ptr == '.';
	for (; i < FAT_FILE_FULLNAME_MAX; i++)
	{
		chr = *ptr;
		if (chr == '/' || chr == 0)
			break;
		fat_fullname[i] = toupper(chr);
		ptr++;
	}
	chr = *ptr;
	if (chr != '/' && chr != 0)
		return (-ENAMETOOLONG);
	for (; i < FAT_FILE_FULLNAME_MAX; i++)
		fat_fullname[i] = ' ';
	return (ptr - filename);
}

bool	fat_file_name_match(const t_phy_fat_file *entry, const char *filename, const char **filename_next)
{
	uint8_t	filename_fat[FAT_FILE_FULLNAME_MAX];
	int		bytes_converted;

	bytes_converted = fat_file_name_convert_strict(filename_fat, filename);
	if (bytes_converted == -ENAMETOOLONG)
		return (false);
	if (memcmp(&entry->name, filename_fat, FAT_FILE_FULLNAME_MAX) != 0)
		return (false);
	*filename_next = filename + bytes_converted;
	*filename_next += **filename_next == '/';
	return (true);
}
