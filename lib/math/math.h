/*
 * File: math.h
 * Author: Joel Souza
 * Date: 2026-03-09
 * Description: Math utilities and function declarations
 */

#ifndef MATH_H
# define MATH_H

# define MIN(a, b) ({ \
	__typeof__(a) _a = (a); \
	__typeof__(b) _b = (b); \
	_a < _b ? _a : _b; \
})

#endif
