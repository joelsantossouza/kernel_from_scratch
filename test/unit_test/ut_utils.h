/*
 * File: ut_utils.h
 * Author: Joel Souza
 * Date: 2026-04-17
 * Description: Utilities macros for unit test framework
 */

#ifndef UT_UTILS_H
# define UT_UTILS_H

// Ensure true and false are bool type
# undef true
# undef false
# define true						((bool)1)
# define false						((bool)0)

// Deeper Stringification
# define UT_STRINGIFY(x)			#x
# define UT_STRINGIFY2(x)			UT_STRINGIFY(x)

// Concatenations
# define UT_CAT(a, b)				a##b
# define UT_CAT2(a, b)				UT_CAT(a, b)
# define UT_CAT3(a, b, c)			UT_CAT2(UT_CAT2(a, b), c)
# define UT_CAT4(a, b, c, d)		UT_CAT2(UT_CAT3(a, b, c), d)
# define UT_CAT5(a, b, c, d, e)		UT_CAT2(UT_CAT4(a, b, c, d), e)
# define UT_CAT6(a, b, c, d, e, f)	UT_CAT2(UT_CAT5(a, b, c, d, e), f)

#endif
