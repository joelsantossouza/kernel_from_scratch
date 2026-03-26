/*
 * File: ctype.h
 * Author: Joel Souza
 * Date: 2026-03-26
 * Description: Character classification and convertion functions
 */

#ifndef CTYPE_H
# define CTYPE_H

static inline
int	islower(int c)
{
	return (c >= 'a' && c <= 'z');
}

static inline
int	toupper(int c)
{
	return (islower(c) ? c - 32 : c);
}

#endif
