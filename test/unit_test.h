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

// Ensure true and false are bool type
# undef true
# undef false
# define true				((bool)1)
# define false				((bool)0)

// Deeper Stringification
# define UT_STRINGIFY2(x)	#x
# define UT_STRINGIFY(x)	UT_STRINGIFY2(x)

// Colors
# define UT_COLOR_SUCCESS	VGA_TEXT_LIGHT_GREEN
# define UT_COLOR_FAILURE	VGA_TEXT_DARK_RED
# define UT_COLOR_LOCATION	VGA_TEXT_WHITE

// Message Prefixes
# define UT_MSGPFX_SUCCESS	"[OK] "
# define UT_MSGPFX_FAILURE	"[KO] "
# define UT_MSGPFX_LOCATION	""

// Messages
# define UT_MSG_LOCATION	__FILE__ ":" UT_STRINGIFY(__LINE__) ": "

// Log
# define UT_LOG(status, msg) \
do \
{ \
	vga_text_print(UT_MSGPFX_LOCATION UT_MSG_LOCATION, UT_COLOR_LOCATION); \
	vga_text_print(UT_MSGPFX_##status msg "\n", UT_COLOR_##status); \
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
		UT_LOG(SUCCESS, #actual " == " #expect); \
	else \
		UT_LOG(FAILURE, #actual " == " #expect); \
} \
while (false)

#endif
