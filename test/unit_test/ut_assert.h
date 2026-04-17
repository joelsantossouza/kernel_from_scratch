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
# include "ut_log.h"

# define UT_EXPECT_EQ(expect, actual) \
do \
{ \
	bool is_equal = _Generic((expect), \
						  bool: (expect) == !!(actual) \
					); \
	if (is_equal) \
		UT_LOG_STATUS(SUCCESS, #actual " == " #expect); \
	else \
		UT_LOG_STATUS(FAILURE, #actual " == " #expect); \
} \
while (false)

#endif
