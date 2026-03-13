/*
 * File: macros.h
 * Author: Joel Souza
 * Date: 2026-03-13
 * Description: Generic kernel macros and compiler helpers for
 * 				performance and readability optimization.
 */

#ifndef KERNEL_MACROS_H
# define KERNEL_MACROS_H

# include <stddef.h>

// Kernel utilities
# define ARRAY_SIZE(array)	(sizeof(array) / sizeof((array)[0]))
# define SAFE_PTRSET(ptr, set) ({ \
	__typeof__(ptr) _ptr = (ptr); \
	__typeof__(set) _set = (set); \
	_ptr != NULL && (*_ptr = _set); \
})

// Compiler optimization
# define LIKELY(x)			__builtin_expect(!!(x), 1)
# define UNLIKELY(x)		__builtin_expect(!!(x), 0)

#endif
