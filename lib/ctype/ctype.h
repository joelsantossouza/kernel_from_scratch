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
	const uint8_t	uc = (uint8_t)c;

	return (uc >= 'a' && uc <= 'z');
}

static inline
int	toupper(int c)
{
	const uint8_t	uc = (uint8_t)c;

	return (islower(uc) ? uc - ASCII_TABLE_CASE_OFFSET : uc);
}

static inline
int	isprint(int c)
{
	const uint8_t	uc = (uint8_t)c;

	return (uc >= ' ' && uc <= '~');
}

static inline
int	iscntrl(int c)
{
	const uint8_t	uc = (uint8_t)c;

	return (uc <= ASCII_UNIT_SEPARATOR || uc == ASCII_DELETE);
}

#endif
