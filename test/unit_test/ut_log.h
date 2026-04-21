/*
 * File: ut_log.h
 * Author: Joel Souza
 * Date: 2026-04-17
 * Description: Unit test status log messages display
 */

#ifndef UT_LOG_H
# define UT_LOG_H

# include <stdbool.h>
# include "drivers/video/text/video_text.h"

/*
 * NAME
 * 	UT_LOG_STATUS, UT_LOG_DESCRIPTION,
 * 	UT_LOG_FATAL - Unit test logging macros
 *
 * DESCRIPTION
 * 	These macros provide formatted logging utilities for the unit
 * 	test framework. Messages are printed with a prefix and color
 * 	based on the specified log type.
 *
 * 	The log type is used with token concatenation (##) to select the
 * 	appropriate message prefix and color (e.g., UT_MSGPFX_ERROR,
 * 	UT_COLOR_ERROR).
 *
 * 	Supported log types include:
 * 		ERROR, SUCCESS, FAILURE, CASE, SUITE, CATEGORY
 *
 * 	UT_LOG_STATUS(type, msg)
 * 		Logs a message including the source location (e.g., file
 * 		and line number), followed by the formatted message.
 *
 * 	UT_LOG_DESCRIPTION(type, msg)
 * 		Logs a message without including source location.
 *
 * 	UT_LOG_FATAL(type, msg)
 * 		Logs a message including source location and halts execution
 * 		by entering an infinite loop.
 * */
# define UT_LOG_STATUS(type, msg) \
do \
{ \
	vga_text_print(UT_LOCATION, UT_COLOR_LOCATION); \
	vga_text_print(UT_MSGPFX_##type, UT_COLOR_##type); \
	vga_text_print(msg, UT_COLOR_##type); \
	vga_text_print("\n", UT_COLOR_##type); \
} \
while (false)

# define UT_LOG_DESCRIPTION(type, msg) \
do \
{ \
	vga_text_print(UT_MSGPFX_##type, UT_COLOR_##type); \
	vga_text_print(msg, UT_COLOR_##type); \
	vga_text_print("\n", UT_COLOR_##type); \
} \
while (false)

# define UT_LOG_FATAL(type, msg) \
do \
{ \
	UT_LOG_STATUS(type, msg); \
	while (true); \
} \
while (false)

#endif
