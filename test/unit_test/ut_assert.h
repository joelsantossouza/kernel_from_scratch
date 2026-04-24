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
 * 	UT_EXPECT_EQ, UT_EXPECT_MEMEQ,
 * 	UT_EXPECT_LT, UT_EXPECT_GT
 * 	- Expression expectation testers
 *
 * DESCRIPTION
 * 	Macros to verify whether an actual expression satisfies an
 * 	expected condition.
 *
 * 	Comparison behavior may be selected according to the type of
 * 	the expected expression using _Generic.
 *
 * 	UT_EXPECT_EQ(expect, actual):
 * 		Verifies equality between two expressions.
 *
 * 	UT_EXPECT_MEMEQ(expect, actual, size):
 * 		Verifies equality between two memory regions by comparing
 * 		exactly 'size' bytes using memcmp().
 *
 * 	UT_EXPECT_LT(limit, actual):
 * 		Verifies whether actual is less than limit.
 *
 * 	UT_EXPECT_GT(limit, actual):
 * 		Verifies whether actual is greater than limit.
 *
 * 	The macros log SUCCESS if the condition evaluates to true,
 * 	or FAILURE otherwise. Logged messages include the stringified
 * 	expressions.
 *
 * 	On FAILURE, execution continues normally. This differs from
 * 	the UT_ASSERT_* family, which halts execution.
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

# define UT_EXPECT_LT(limit, actual) \
do \
{ \
	bool is_less = _Generic((limit), \
						 default: (actual) < (limit) \
					); \
	if (is_less) \
		UT_LOG_STATUS(SUCCESS, #actual " < " #limit); \
	else \
		UT_LOG_STATUS(FAILURE, #actual " < " #limit); \
} \
while (false)

# define UT_EXPECT_GT(limit, actual) \
do \
{ \
	bool is_greater = _Generic((limit), \
							default: (actual) > (limit) \
						); \
	if (is_greater) \
		UT_LOG_STATUS(SUCCESS, #actual " > " #limit); \
	else \
		UT_LOG_STATUS(FAILURE, #actual " > " #limit); \
} \
while (false)

#endif
