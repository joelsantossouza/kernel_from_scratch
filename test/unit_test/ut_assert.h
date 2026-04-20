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
 * 	UT_EXPECT_EQ - Expression equality tester
 *
 * DESCRIPTION
 * 	Tests whether the actual value is equal to the expected value.
 *
 * 	The comparison behavior is selected based on the type of the
 * 	expected value using _Generic. Currently, the following types
 * 	are supported:
 * 		- bool: compares (bool)(expect) == (bool)(actual)
 * 		- char *: compares strcmp(expect, actual) == 0
 * 		- default: compares (expect) == (actual)
 *
 * 	The macro logs the result as SUCCESS if the comparison evaluates
 * 	to true, or FAILURE otherwise. The logged message includes the
 * 	stringified expressions:
 * 		"actual == expect"
 * */
# define UT_EXPECT_EQ(expect, actual) \
do \
{ \
	bool is_equal = _Generic((expect), \
						  bool: ((bool)(uint32_t)(expect)) == ((bool)(uint32_t)(actual)), \
						  char *: strcmp((const char *)(expect), (const char *)(actual)) == 0, \
						  default: (expect) == (actual) \
					); \
	if (is_equal) \
		UT_LOG_STATUS(SUCCESS, #actual " == " #expect); \
	else \
		UT_LOG_STATUS(FAILURE, #actual " == " #expect); \
} \
while (false)

#endif
