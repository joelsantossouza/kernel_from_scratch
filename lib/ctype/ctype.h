/*
 * File: ctype.h
 * Author: Joel Souza
 * Date: 2026-03-26
 * Description: Character classification and convertion functions
 */

#ifndef CTYPE_H
# define CTYPE_H

# include <stdint.h>
# include "ascii.h"

static inline
int	islower(int c)
{
	return (c >= 'a' && c <= 'z');
}

static inline
int	toupper(int c)
{
	return (islower(c) ? c - ASCII_TABLE_CASE_OFFSET : c);
}

static inline
int	isprint(int c)
{
	return (c >= ' ' && c <= '~');
}

static inline
int	iscntrl(int c)
{
	return ((uint32_t)c <= ASCII_UNIT_SEPARATOR || c == ASCII_DELETE);
}

#endif
