/*
 * File: unit_test.h
 * Author: Joel Souza
 * Date: 2026-04-14
 * Description: Assertion and expect macros utilities to track
 * 				successes and failures with clear outputs.
 */

#ifndef UNIT_TEST_H
# define UNIT_TEST_H

# include <stdbool.h>
# include "drivers/video/text/video_text.h"

// Colors
# define UT_COLOR_SUCCESS	VGA_TEXT_LIGHT_GREEN
# define UT_COLOR_FAILURE	VGA_TEXT_DARK_RED
# define UT_COLOR_LOCATION	VGA_TEXT_WHITE

// Prefixes
# define UT_PREFIX_SUCCESS	"[OK] - "
# define UT_PREFIX_FAILURE	"[KO] - "
# define UT_PREFIX_LOCATION	__FILE__ ":" __LINE__ ": "

// Log
# define UT_LOG(status, msg) \
do \
{ \
	vga_text_print(UT_PREFIX_LOCATION, UT_COLOR_LOCATION); \
	vga_text_print(UT_PREFIX_##status msg, UT_PREFIX_##status); \
} \
while (false)

// Assertion
# define UT_EXPECT_EQ(expect, actual) \
do \
{ \
	bool is_equal = _Generic((expect), \
						  bool: (expect) == !!(actual) \
					); \
	if (is_equal) \
		UT_LOG(SUCCESS, #expect " == " #actual); \
	else \
		UT_LOG(FAILURE, #expect " != " #actual); \
} \
while (false)

#endif
