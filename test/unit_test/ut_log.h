/*
 * File: ut_log.h
 * Author: Joel Souza
 * Date: 2026-04-17
 * Description: Unit test status log messages display
 */

#ifndef UT_LOG_H
# define UT_LOG_H

# include <stdbool.h>

# define UT_LOG_STATUS(type, msg) \
do \
{ \
	vga_text_print(UT_LOCATION, UT_COLOR_LOCATION); \
	vga_text_print( \
		UT_MSGPFX_##type msg "\n", UT_COLOR_##type \
	); \
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
