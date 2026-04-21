/*
 * File: ut_assert.h
 * Author: Joel Souza
 * Date: 2026-04-17
 * Description: Macro to test expression, tracking results and display useful test
 * 				result on screen
 */

#ifndef UT_ASSERT_H
# define UT_ASSERT_H

# include <stdbool.h>
# include "string/string.h"
# include "ut_log.h"

/*
 * NAME
 * 	UT_EXPECT_EQ, UT_EXPECT_MEMEQ - Expression equality testers
 *
 * DESCRIPTION
 * 	Macros to test whether the actual value matches the expected value.
 *
 * 	UT_EXPECT_EQ(expect, actual):
 * 		Compares two expressions for equality. The comparison behavior
 * 		is selected based on the type of the expected value using _Generic:
 * 			- bool: compares (bool)(expect) == (bool)(actual)
 * 			- default: compares (expect) == (actual)
 *
 * 	UT_EXPECT_MEMEQ(expect, actual, size):
 * 		Compares two memory regions for equality. Both expect and actual
 * 		are interpreted as pointers to memory, and memcmp() is used to
 * 		compare 'size' bytes.
 *
 * 	The macros log the result as SUCCESS if the comparison evaluates
 * 	to true, or FAILURE otherwise. The logged message includes the
 * 	stringified expressions:
 * 		"actual == expect"
 * */
# define UT_EXPECT_EQ(expect, actual) \
do \
{ \
	bool is_equal = _Generic((expect), \
						  bool: ((bool)(uint32_t)(expect)) == ((bool)(uint32_t)(actual)), \
						  default: (expect) == (actual) \
					); \
	if (is_equal) \
		UT_LOG_STATUS(SUCCESS, #actual " == " #expect); \
	else \
		UT_LOG_STATUS(FAILURE, #actual " == " #expect); \
} \
while (false)

# define UT_EXPECT_MEMEQ(expect, actual, size) \
do \
{ \
	bool is_equal = memcmp((const void *)(expect), (const void *)(actual), (uint32_t)size) == 0; \
	if (is_equal) \
		UT_LOG_STATUS(SUCCESS, #actual " == " #expect); \
	else \
		UT_LOG_STATUS(FAILURE, #actual " == " #expect); \
} \
while (false)

#endif
