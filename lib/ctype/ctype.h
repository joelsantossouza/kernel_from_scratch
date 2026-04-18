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

/*
 * NAME
 * 	islower, isprint, iscntrl -
 * 	ASCII character classification functions
 *
 * DESCRIPTION
 * 	These functions classify characters based on their ASCII
 * 	values.
 *
 * 	Each function takes an integer value representing a character
 * 	and returns whether it belongs to a specific classification:
 *
 * 		islower → lowercase letters ('a' to 'z')
 * 		isprint → printable characters (' ' to '~')
 * 		iscntrl → control characters (0x00–0x1F and 0x7F)
 *
 * 	The input is evaluated directly as an int32 value without
 * 	locale considerations or unsigned casting.
 *
 * RETURN VALUE
 * 	Returns a non-zero value if the character matches the
 * 	classification, or 0 otherwise.
 * */
static inline
int	islower(int c)
{
	return (c >= 'a' && c <= 'z');
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

/*
 * NAME
 * 	toupper, tolower - Convert character case
 *
 * DESCRIPTION
 * 	These functions convert the case of ASCII characters:
 *
 * 		toupper(c) converts lowercase letters ('a'–'z')
 * 		to their uppercase equivalents.
 *
 * 		tolower(c) converts uppercase letters ('A'–'Z')
 * 		to their lowercase equivalents.
 *
 * 	If the input character does not belong to the expected
 * 	range, it is returned unchanged.
 *
 * 	The conversion relies on the fixed offset between
 * 	uppercase and lowercase letters in the ASCII table.
 *
 * RETURN VALUE
 * 	Returns the converted character if a case change is
 * 	applicable, or the original value otherwise.
 * */
static inline
int	toupper(int c)
{
	return (islower(c) ? c - ASCII_TABLE_CASE_OFFSET : c);
}

#endif
