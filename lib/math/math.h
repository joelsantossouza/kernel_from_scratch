/*
 * File: math.h
 * Author: Joel Souza
 * Date: 2026-03-09
 * Description: Math utilities and function declarations
 */

#ifndef MATH_H
# define MATH_H

# include <stdbool.h>
# include <stdint.h>

/*
 * NAME
 * 	MIN, MAX - Compute the minimum or maximum of two values
 *
 * DESCRIPTION
 * 	These macros evaluate and return the minimum or maximum of
 * 	two given values:
 *
 * 		MIN(a, b) → returns the smaller value
 * 		MAX(a, b) → returns the larger value
 *
 * 	If both values are equal, that value is returned.
 *
 * 	Arguments are evaluated exactly once by storing them in
 * 	temporary variables. This allows safe usage with expressions
 * 	that have side effects.
 *
 * 	Example:
 *
 * 		int ten = 10;
 * 		MIN(ten++, 12) → returns 10
 *
 * 	This implementation relies on GCC/Clang statement expression
 * 	extensions `({ ... })` and is not part of standard C.
 *
 * RETURN VALUE
 * 	Returns the minimum or maximum of the two input values.
 * */
# define MIN(a, b) ({ \
	__typeof__(a) _a = (a); \
	__typeof__(b) _b = (b); \
	_a < _b ? _a : _b; \
})

/*
 * NAME
 * 	align_down, align_up - Unsigned 32-bit alignment functions
 *
 * DESCRIPTION
 * 	These functions round an unsigned 32-bit integer up or down
 * 	to the nearest multiple of 'align'.
 *
 * 	align_down(n, align)
 * 		Rounds n down to the nearest aligned boundary.
 *
 * 	align_up(n, align)
 * 		Rounds n up to the nearest aligned boundary.
 *
 * 	'align' must be greater than 0. Otherwise, the modulo operation
 * 	results in undefined behavior.
 *
 * RETURN VALUE
 * 	Returns the aligned result.
 * */
static inline
uint32_t	align_down(uint32_t n, uint32_t align)
{
	return (n - n % align);
}

static inline
uint32_t	align_up(uint32_t n, uint32_t align)
{
	return (align_down(n + align - 1, align));
}

/*
 * NAME
 * 	is_pow2 - Check if a number is a power of 2
 *
 * DESCRIPTION
 * 	Determines whether the given unsigned 32-bit integer is
 * 	a power of 2.
 *
 * RETURN VALUE
 * 	Returns true if n is a power of 2, false otherwise.
 * */
static inline
bool	is_pow2(uint32_t n)
{
	return (n != 0 && (n & (n - 1)) == 0);
}

#endif
