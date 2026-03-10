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

# define ALIGN_DOWN(n, align) ({ \
	__typeof__(n) _n = (n); \
	__typeof__(align) _align = (align); \
	_n - _n % _align; \
})

# define ALIGN_UP(n, align) ({ \
	__typeof__(n) _n = (n); \
	__typeof__(align) _align = (align); \
})

#endif
